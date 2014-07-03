<?php

/**
 * @file
 * Base class for OAuth2 Authentication clients.
 */

/**
 * Description of OAuth2AuthenticationClient class.
 *
 * The OAuth2AuthenticationClient class is used to authenticate users based on
 * valid accesss tokens provided by an OAuth2 server.  Once users are logged in,
 * their sessions will contain the tokens necessary for gaining access to remote
 * resources.  Valid remote users who do not exist locally will be created.
 *
 * Feel free to subclass in order to override anything done here.  If you do,
 * make sure to add the new class to the configuration.
 */
class OAuth2AuthenticationClient {

  /**
   * The username of the user whose access is being requested.
   */
  protected $username = NULL;

  /**
   * The password of the user whose access is being requested.
   */
  protected $password = NULL;

  /**
   * Construct an OAuth2\OAuth2AuthenticationClient object.
   *
   * @param string $username
   *   The username of the user whose authentication is requested.
   * @param string $password
   *   The username of the user whose authentication is requested.
   */
  public function __construct($username, $password) {
    // Set the username and password for later use.
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Determines if a user with a provided name and password exists remotely.
   *
   * @return
   *   TRUE if the user exists remotely; FALSE otherwise.
   */
  public function userExistsRemotely() {

    // Configure the OAuth2 client.
    $oauth2_config = array(
      'auth_flow'      => 'user-password',
      'token_endpoint' => variable_get('oauth2_authentication_token_endpoint', ''),
      'client_id'      => variable_get('oauth2_authentication_client_id', ''),
      'client_secret'  => variable_get('oauth2_authentication_client_secret', ''),
      'scope'          => variable_get('oauth2_authentication_scope', ''),
      'username'       => $this->username,
      'password'       => $this->password,
    );

    try {
      // Create an OAuth2 client and attempt to get an access token.  If we
      // aren't able to, we'll end up in the catch stanza as an exception will
      // be thrown.
      $oauth2_client = new OAuth2\Client($oauth2_config);
      $user_exists_remotely = !empty($oauth2_client->getAccessToken());
    }
    catch (Exception $e) {
      // We couildn't get an access token for this user so it must not be valid.
      $user_exists_remotely = FALSE;
    }

    // Return the result.
    return $user_exists_remotely;
  }

  /**
   * Create a new user based on the successful validation of a remote user.
   *
   * This function creates a new local Drupal user if a corresponding remote
   * user exists, but doesn't exist here yet.
   *
   * @return
   *   A fully-loaded $user object upon successful creation or FALSE on failure.
   */
  public function createUserLocally() {

    // Get the user's e-mail address from some remote service.
    $email = $this->getUserEmailAddress();

    // Create a list of user information.
    $user = array(
      'name'   => $this->username,
      'pass'   => $this->password,
      'mail'   => $email,
      'status' => 1,
      'init'   => $email,
      'roles'  => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      ),
    );

    // Save the new user.
    return user_save(NULL, $user);
  }

  /**
   * Fetches the e-mail address of the user to be created.
   *
   * This should be overridden.  Otherwise, your users won't have e-mail
   * addresses.
   *
   * @return
   *   The user's e-mail address.
   */
  protected function getUserEmailAddress() {
    return '';
  }

}

