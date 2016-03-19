<?php

/*
 * Check the updated date of the content on the server.
*/
function content_dashboard_sync_settings() {
  $form = array();

  // Get list of all the content types.
  $content_types = node_type_get_types();

  foreach ($content_types as $content_type_name => $value) {
    $form_name = 'content_dashboard_updated_' . $content_type_name . '_since';
    $form[$form_name] = array(
      '#type' => 'textfield',
      '#title' => t('Number of days to retrieve updated ') . $content_type_name,
      '#default_value' => variable_get($form_name, 10),
      '#element_validate' => array('element_validate_integer_positive'),
    );
  }

  return system_settings_form($form);
}

/*
 * Server Configuration.
*/
function content_dashboard_config_form() {
  $form = array();

  // URL of the server
  $form['content_dashboard_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Url'),
    '#description' => t('Enter the URL of the server from which you want to import the content. e.g http://server.com'),
    '#default_value' => variable_get('content_dashboard_server_url', ''),
  );

  // Server username for Authentication.
  $form['content_dashboard_server_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator username on Server'),
    '#default_value' => variable_get('content_dashboard_server_username', ''),
  );

  // Server username password for Authentication.
  $form['content_dashboard_server_password'] = array(
    '#type' => 'password',
    '#title' => t('Administrator password on Server'),
    '#default_value' => variable_get('content_dashboard_server_password', ''),
  );

  // List of languages whose content you wish to import from the server.
  $form['content_dashboard_server_language'] = array(
    '#type' => 'textfield',
    '#title' => t('List of languages'),
    '#description' => t('List of languages whose content you want to import from the content Server. Use a comma to separate multiple languages.'),
    '#default_value' => variable_get('content_dashboard_server_language'),
  );

  return system_settings_form($form);
}
