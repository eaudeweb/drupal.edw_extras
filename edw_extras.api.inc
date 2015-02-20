<?php

interface EdwSecurityPlugin {

  /**
   * @return boolean
   *   TRUE if plugin is enabled and can be used
   */
  function is_enabled();

  /**
   * @return string
   *   Plugin version
   */
  function get_version();

  /**
   * @return string
   *   Unique plugin machine name
   */
  function get_machine_name();

  /**
   * @return string
   *   Plugin description
   */
  function get_description();

  /**
   * Compute security integrity for the registry table.
   *
   * @return \stdClass
   *   Object populated with security information
   */
  function get_data();

  /**
   * @param array $data
   *   Data to check, previously generated with get_data().
   *
   * @return boolean
   *   Return FALSE if security check failed
   */
  function check($data);
}


function hook_edw_extras_security_plugin() {
  return array('EdwSecurityRegistryPlugin');
}