<?php

/*
 * Check the updated date of the content on the server.
*/
function content_dashboard_sync_settings() {
  $form = array();

  // Get list of all the content types.
  $content_types = node_type_get_types();

  foreach ($content_types as $content_type_name => $value) {
  	$form_name = 'updated_' . $content_type_name . '_since';
  	$form[$form_name] = array(
	    '#type' => 'textfield',
	    '#title' => t('Number of days to retrieve updated ') . $content_type_name,
	    '#default_value' => variable_get($form_name, 10),
	    '#element_validate' => array('element_validate_integer_positive'),
	  );
  }

  return system_settings_form($form);
}