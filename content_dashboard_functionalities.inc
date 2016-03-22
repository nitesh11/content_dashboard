<?php

/**
 * [retrieve_session_cookie description]
 * @param  [type] $url           [description]
 * @param  [type] $login_details [JSON - string, containing username and password]
 * @return [type]                [description]
 */
function retrieve_session_cookie() {
  $login_details = '{"username" : "' . variable_get('content_dashboard_server_username') . '", "password" : "' . variable_get('content_dashboard_server_password') . '"}';
  $url = variable_get('content_dashboard_server_url') . '/test/user/login';
  $options = array(
    'method' => 'POST',
    'headers' => array('Content-Type' => 'application/json'),
    'data' => $login_details ,
  );

  $result = drupal_json_decode(drupal_http_request($url,$options)->data);
  if (empty($result) || (isset($result[0]) && $result[0] == 'Wrong username or password.')) {
    drupal_set_message(t('Server Url is not setup for fetching content. You can configure it ') . l(t('Here') , 'admin/config/content-dashboard/config_form') . '.', 'warning');
    return FALSE;
  }
  else {
    $sessid = $result["sessid"];
    $session_name = $result["session_name"];
    $cookie = $session_name . '=' . $sessid;
    return $cookie;
  }
}

/*
 * Get the Node count of the content type.
 */
function get_new_node_count($type, $server_url, $cookie) {
  $count = 0;
  $client_uuid = array();
  $basic_node_list = array();
  $language = explode(',', variable_get('content_dashboard_server_language', ''));

  foreach ($language as $lang) {
    $temp = drupal_json_decode(get_basic_node_info($type, $cookie, $lang)->data);
    $basic_node_list = array_merge($basic_node_list,$temp);
  }

  $query = db_select("node", "n")
    ->fields("n", array("uuid"))
    ->condition("type", $type)
    ->execute();

  while ($row = $query->fetchField()) {
    $client_uuid[] = $row;
  }

  if (!empty($basic_node_list)) {
    foreach ($basic_node_list as $key => $node) {
      if (!empty($client_uuid) && in_array($node["uuid"], $client_uuid)) {
        unset($basic_node_list[$key]);
        continue;
      }
      else $count++;
    }
  }
  return $count;
}

/*
 * Get the basic information of the node.
 */
function get_basic_node_info($type, $cookie, $language) {
  $url = variable_get('content_dashboard_server_url') . '/test/node';
  $data = array(
    'parameters[type]' => $type,
    'parameters[status]' => 1,
  );

  $full_url = url($url, array('query' => $data));
  $options = array('method' => 'GET');
  $options['headers']['Cookie'] = $cookie;
  return drupal_http_request($full_url, $options);
}

/*
 * Importing taxonomy terms.
 */
function add_taxonomy_term($term_uuid, $options, $server_url) {

  $term_details = drupal_json_decode(drupal_http_request($server_url . '/test/taxonomy_term/' . $term_uuid, $options)->data);

  // Mapping array for Taxonomy terms.
  $mapping = array(
    "name" => "name",
    "description" => "description",
    "uuid" => "uuid",
    "format" => "format",
  );

  // Check if the term already exists or not.
  $tid = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid'))
    ->condition('uuid', $term_uuid)
    ->execute()->fetchField();

  // Load the term if the term exists.
  if ($tid) {
    $term = taxonomy_term_load($tid);
  }
  else {
    $term = new stdClass();
  }

  // Updating the terms.
  foreach($mapping as $key) {
    $term->$key = $term_details[$key];
  }

  $term->vocabulary_machine_name = $term_details["vocabulary_machine_name"];

  // Getting the vocab ID on the client side.
  $vocab_id = db_select('taxonomy_vocabulary', 'vocab')
    ->fields('vocab', array('vid'))
    ->condition('machine_name', $term_details["vocabulary_machine_name"])
    ->execute()->fetchObject();

  $term->vid = $vocab_id->vid;

  content_dashboard_taxonomy_term_save($term);
  return $term->tid;
}

/*
 * Creating/Updating the taxonomy.
 */
function content_dashboard_taxonomy_term_save($term) {
  if(isset($term->name) && !empty($term->name) && strlen($term->name)>0){
    taxonomy_term_save($term);
  }
  else {
    drupal_set_message(t("Can't save term"), 'error', FALSE);
  }
}