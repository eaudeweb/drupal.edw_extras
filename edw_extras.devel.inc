<?php

/**
 * Simple wrapper to configure a search_api server (tested with Solr).
 */
function edw_extras_search_api_solr_configure($machine_name, $options = array()) {
  if (!function_exists('search_api_server_load')) {
    return TRUE;
  }
  $cfg = array_merge(
    array(
      'name' => 'Solr server',
      'enabled' => 1,
      'description' => 'Search server',
      'scheme' => 'http',
      'host' => 'localhost',
      'port' => '8983',
      'http_user' => '',
      'http_password' => '',
      'excerpt' => NULL,
      'retrieve_data' => NULL,
      'highlight_data' => NULL,
      'skip_schema_check' => NULL,
      'solr_version' => '',
      'http_method' => 'AUTO',
    ), $options
  );
  if (empty($cfg['path'])) {
    return FALSE;
  }
  /** @var SearchApiServer $server */
  if ($server = search_api_server_load($machine_name)) {
    $server->name = $cfg['name'];
    $server->enabled = $cfg['enabled'];
    $server->description = $cfg['description'];
    $server->options = array(
      'scheme' => $cfg['scheme'],
      'host' => $cfg['host'],
      'port' => $cfg['port'],
      'path' => $cfg['path'],
      'http' => array(
        'http_user' => $cfg['http_user'],
        'http_pass' => $cfg['http_pass'],
      ),
      'excerpt' => $cfg['excerpt'],
      'retrieve_data' => $cfg['retrieve_data'],
      'highlight_data' => $cfg['highlight_data'],
      'skip_schema_check' => $cfg['skip_schema_check'],
      'solr_version' => $cfg['solr_version'],
      'http_method' => $cfg['http_method'],
    );
    $server->save();
  }
  return TRUE;
}
