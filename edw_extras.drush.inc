<?php


function edw_extras_drush_command() {
  return array(
    'edw-security-generate' => array(
      'description' => "Saves single block configuration (ie move block to a region)",
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'options' => array(
        'pretty-print'  => 'Generate JSON file with pretty-print format',
      ),
    ),
    'edw-security-check' => array(
      'description' => "Check the security against an pre-existing file",
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'options' => array(),
    )
  );
}

function drush_edw_extras_edw_security_generate() {
  $options = 0;
  if (drush_get_option('pretty-print', FALSE)) {
    $options = JSON_PRETTY_PRINT;
  }
  $content = EdwSecurity::integrity_check_generate($options);
  echo json_encode($content, $options);
}

function drush_edw_extras_edw_security_check($file = NULL) {
  EdwSecurity::integrity_check_from_file($file);
}
