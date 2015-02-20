<?php

/**
 * Class EdwSecurity does system security checks and reports
 */
class EdwSecurity {

  /**
   * Generate security information.
   *
   * The output should be stored in a file read-only to Drupal and Apache
   *
   * @return array
   *   Security information
   */
  public static function integrity_check_generate() {
    $ret = array();
    $ret['date'] = date(DATE_ISO8601);
    $plugins = module_invoke_all('edw_extras_security_plugin');
    foreach($plugins as $klass) {
      $plugin = new $klass();
      if ($plugin->is_enabled()) {
        $machine_name = $plugin->get_machine_name();
        $data = $plugin->get_data();
        $data['version'] = $plugin->get_version();
        $ret[$machine_name] = $data;
      }
    }
    return $ret;
  }

  /**
   * Read security information previously generated and compare with current
   *
   * @param string $file
   *   File to read data from
   *
   * @return bool
   *   TRUE if security is fine, FALSE if security check failed
   */
  public static function integrity_check_from_file($file) {
    if(!is_file($file) || !is_readable($file) || !json_decode(file_get_contents($file))) {
      $msg = dt('Failed to read security file !file', array('!file' => $file));
      drupal_set_message($msg);
      if (function_exists('drush_set_error')) {
        drush_set_error($msg);
      }
      return FALSE;
    }
    else {
      $security = json_decode(file_get_contents($file), TRUE);
      if (!self::integrity_check($security)) {
        $msg = dt('Security check failed for file !file', array('!file' => $file));
        drupal_set_message($msg);
        if (function_exists('drush_set_error')) {
          drush_set_error($msg);
        }
        return FALSE;
      }
    }
    return TRUE;
  }


  /**
   * Do the actual integrity check
   *
   * @param array $data
   *   Security data previously generated
   *
   * @return bool
   *   TRUE if security is fine, FALSE if security check failed
   */
  public static function integrity_check($data) {
    $failed = FALSE;
    $plugins = module_invoke_all('edw_extras_security_plugin');
    foreach($plugins as $klass) {
      $plugin = new $klass();
      if ($plugin->is_enabled()) {
        $machine_name = $plugin->get_machine_name();
        if(array_key_exists($machine_name, $data)) {
          $result = $plugin->check($data[$machine_name]);
          if (!$result) {
            $failed = TRUE;
          }
        }
        else {
          drush_log(dt('Cannot find key !key in security file. Possible the plugin was not previously installed when security file was generated', array('!key' => $machine_name)));
        }
      }
    }
    return $failed;
  }
}

/**
 * Class EdwSecurityRegistryPlugin implements security check for registry table.
 */
class EdwSecurityRegistryPlugin implements EdwSecurityPlugin {

  public function is_enabled() {
    return TRUE;
  }

  public function get_version() {
    return "1.0.0";
  }

  public function get_machine_name() {
    return 'edw_extras_registry';
  }

  public function get_description() {
    return t('This security plugin checks the security of the registry table');
  }

  /**
   * Compute security integrity for the registry table.
   *
   * @return \stdClass
   *   Object populated with security information
   */
  public function get_data() {
    $ret = array();
    $query = db_select('registry', 'r')->fields('r');
    $rows = $query->execute()->fetchAllAssoc('filename');
    $ret['count'] = count($rows);
    $ret['files'] = array();
    foreach ($rows as $filename => $row) {
      $filename_abs = DRUPAL_ROOT . '/' . $filename;
      $file = array(
        'filename' => $filename,
        'size' => filesize($filename_abs),
        'checksum' => md5(file_get_contents($filename_abs)),
        'mtime' => filemtime($filename_abs),
        'ctime' => filectime($filename_abs)
      );
      $ret['files'][$filename] = $file;
    }
    return $ret;
  }

  public function check($original) {
    $current = self::get_data();
    $failed = FALSE;
    // Check item counts
    if ($original['count'] != $current['count']) {
      return FALSE;
    }
    // Check each item
    foreach($original['files'] as $filename => $file_data) {
      $file_current = $current['files'][$filename];
      if($file_current['size'] != $file_data['size']) {
        drush_set_error(dt(
          'Failed size security check for file !file (!is bytes, was !was bytes)',
          array('!file' => $filename, '!is' => $file_current['size'], '!was' => $file_data['size'])
        ));
        $failed = TRUE;
      }
      if($file_current['checksum'] != $file_data['checksum']) {
        drush_set_error(dt(
          'Failed checksum security check for file !file (!is, was !was)',
          array('!file' => $filename, '!is' => $file_current['checksum'], '!was' => $file_data['checksum'])
        ));
        $failed = TRUE;
      }
      if($file_current['mtime'] != $file_data['mtime']) {
        drush_set_error(dt(
          'Failed mtime security check for file !file (!is, was !was)',
          array('!file' => $filename, '!is' => $file_current['mtime'], '!was' => $file_data['mtime'])
        ));
        $failed = TRUE;
      }
      if($file_current['ctime'] != $file_data['ctime']) {
        drush_set_error(dt(
          'Failed ctime security check for file !file (!is, was !was)',
          array('!file' => $filename, '!is' => $file_current['ctime'], '!was' => $file_data['ctime'])
        ));
        $failed = TRUE;
      }
    }
    return $failed;
  }
}
