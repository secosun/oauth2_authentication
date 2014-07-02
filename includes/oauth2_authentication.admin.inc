<?php
/**
 * @file
 * Administrative UI and functions for the OAuth2 Authentication module.
 */

/**
 * Form builder: Main administrative form.
 */
function oauth2_authentication_admin_form($form, &$form_state) {

  // Add a text field for the token endpoint.
  $form['oauth2_authentication_token_endpoint'] = array(
    '#title' => t('Token endpoint URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('oauth2_authentication_token_endpoint', 'https://idp.example.com/token'),
    '#description' => t('A valid URL from where access tokens can be retrieved.'),
  );

  // Add a text field for the client ID.
  $form['oauth2_authentication_client_id'] = array(
    '#title' => t('Client ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('oauth2_authentication_client_id', ''),
    '#description' => t('This sites\'s ID for authenticating to the token server.'),
  );

  // Add a text field for the client secret.
  $form['oauth2_authentication_client_secret'] = array(
    '#title' => t('Client secret'),
    '#type' => 'textfield',
    '#default_value' => variable_get('oauth2_authentication_client_secret', ''),
    '#description' => t('This site\'s secret used for authenticating to the token server.'),
  );

  // Add a text field for the scope.
  $form['oauth2_authentication_scope'] = array(
    '#title' => t('Scope'),
    '#type' => 'textfield',
    '#default_value' => variable_get('oauth2_authentication_scope', ''),
    '#description' => t('The scope of the access request.'),
  );

  // Add a text field for the name of the class to handle the authentication.
  $form['oauth2_authentication_class'] = array(
    '#title' => t('Class'),
    '#type' => 'textfield',
    '#default_value' => variable_get('oauth2_authentication_class', 'OAuth2AuthenticationClient'),
    '#description' => t('The name of the class that handles the authentication.  If you have extended the default one to make modifications, enter it here.'),
  );

  // Configure the form submission button.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validation handler for oauth2_authentication_admin_form.
 */
function oauth2_authentication_admin_form_validate($form, &$form_state) {

  // Fetch the form values.
  $token_endpoint = $form_state['values']['oauth2_authentication_token_endpoint'];
  $client_id      = $form_state['values']['oauth2_authentication_client_id'];
  $client_secret  = $form_state['values']['oauth2_authentication_client_secret'];
  $scope          = $form_state['values']['oauth2_authentication_scope'];
  $class          = $form_state['values']['oauth2_authentication_class'];

  // Ensure that the token endpoint is a valid URL.
  if (!valid_url($token_endpoint, TRUE)) {
    form_set_error('oauth2_authentication_token_endpoint', 'The token endpoint is not a valid URL.');
  }

  // Ensure that the client ID has been set.
  if (empty($client_id)) {
    form_set_error('oauth2_authentication_client_id', 'The client ID cannot be empty.');
  }

  // Ensure that the client secret has been set.
  if (empty($client_secret)) {
    form_set_error('oauth2_authentication_client_secret', 'The client secret cannot be empty.');
  }

  // Don't bother checking for a valid scope parameter as it's optional.

  // Ensure that the class has been defined.
  if (!class_exists($class)) {
    form_set_error('oauth2_authentication_class', 'The authentication-handling class has not been defined.  It must exist before it can be used.');
  }
}

/**
 * Submit handler for oauth2_authentication_admin_form.
 */
function oauth2_authentication_admin_form_submit($form, &$form_state) {

  // Fetch the form values.
  $token_endpoint = $form_state['values']['oauth2_authentication_token_endpoint'];
  $client_id      = $form_state['values']['oauth2_authentication_client_id'];
  $client_secret  = $form_state['values']['oauth2_authentication_client_secret'];
  $scope          = $form_state['values']['oauth2_authentication_scope'];
  $class          = $form_state['values']['oauth2_authentication_class'];

  // Save the token endpoint.
  if (!empty($token_endpoint)) {
    variable_set('oauth2_authentication_token_endpoint', $token_endpoint);
  }

  // Save the client ID.
  if (!empty($client_id)) {
    variable_set('oauth2_authentication_client_id', $client_id);
  }

  // Save the client secret.
  if (!empty($client_secret)) {
    variable_set('oauth2_authentication_client_secret', $client_secret);
  }

  // Save the scope.
  if (!empty($scope)) {
    variable_set('oauth2_authentication_scope', $scope);
  }

  // Save the class.
  if (!empty($class)) {
    variable_set('oauth2_authentication_class', $class);
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
