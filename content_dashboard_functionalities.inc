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
  $client_uuid = array();
  $count = 0;
  $language = explode(',', variable_get('content_dashboard_server_language', ''));
  $basic_node_list = array();
  foreach ($language as $lang) {
    $temp = drupal_json_decode(get_basic_node_info($type, $cookie, $lang)->data);
    $basic_node_list = array_merge($basic_node_list,$temp);
  }
  $query = db_select("node", "n")->fields("n", array("uuid"))->condition("type", $type)->execute();
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
  $data = array('parameters[type]' => $type,'parameters[status]' => 1);

  $full_url = url($url, array('query' => $data));
  $options = array('method' => 'GET');
  $options['headers']['Cookie'] = $cookie;
  return drupal_http_request($full_url, $options);
}
