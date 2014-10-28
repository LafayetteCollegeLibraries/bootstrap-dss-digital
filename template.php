<?php

/**
 * @file template.php
 * @author griffinj@lafayette.edu
 * This file contains the primary theme hooks found within any given Drupal 7.x theme
 *
 * @todo Implement some Drupal theming hooks
 */

  // Includes functions to create Islandora Solr blocks.
require_once dirname(__FILE__) . '/includes/blocks.inc';
require_once dirname(__FILE__) . '/includes/forms.inc';
require_once dirname(__FILE__) . '/includes/menus.inc';
require_once dirname(__FILE__) . '/includes/dss_mods.inc';
require_once dirname(__FILE__) . '/includes/dss_dc.inc';
require_once dirname(__FILE__) . '/includes/pager.inc';
require_once dirname(__FILE__) . '/includes/islandora_solr.inc';
require_once dirname(__FILE__) . '/includes/islandora_basic_collection.inc';

function bootstrap_dss_digital_preprocess_node(&$vars) {

  if($vars['page']) {

    // Add header meta tag for IE to head
    global $base_url;
    $meta_element_open_graph_type = array('#type' => 'html_tag',
					  '#tag' => 'meta',
					  '#attributes' => array('property' =>  'og:type',
								 'content' => 'article'),
					  );

    $meta_element_open_graph_url = array('#type' => 'html_tag',
					 '#tag' => 'meta',
					 '#attributes' => array('property' =>  'og:url',
								'content' => $base_url . '/' . drupal_get_path_alias()
								),
					 );

    $meta_element_open_graph_author = array('#type' => 'html_tag',
					    '#tag' => 'meta',
					    '#attributes' => array('property' =>  'og:author',
								   'content' => 'https://www.facebook.com/LafayetteCollegeLibrary',
								   )
					    );

    $meta_element_open_graph_title = array('#type' => 'html_tag',
					   '#tag' => 'meta',
					   '#attributes' => array('property' =>  'og:title',
								  'content' => $vars['title'],
								  )
					   );

    // For all <meta> elements
    $meta_elements = array(
			   'meta_element_open_graph_type' => $meta_element_open_graph_type,
			   'meta_element_open_graph_url' => $meta_element_open_graph_url,
			   'meta_element_open_graph_author' => $meta_element_open_graph_author,
			   'meta_element_open_graph_title' => $meta_element_open_graph_title,
			   );
    $meta_elements['meta_element_open_graph_image'] = array('#type' => 'html_tag',
							    '#tag' => 'meta',
							    '#attributes' => array('property' =>  'og:image',
										   'content' => $base_url . '/' . drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/dss_logo_full.png',
										   ),
							    );
    $meta_elements['meta_element_open_graph_site_name'] = array('#type' => 'html_tag',
								'#tag' => 'meta',
								'#attributes' => array('property' =>  'og:site_name',
										       'content' => 'Digital Scholarship Services',
										       )
								);

    foreach($meta_elements as $key => $meta_element) {

      // Add header meta tag for IE to head
      drupal_add_html_head($meta_element, $key);
    }
  }

  /**
   * Implements redirection for the Repository Migration page
   * @todo Refactor
   * Resolves DSSSM-826
   */
  if($vars['node_url'] == '/redirect') {

    drupal_add_js('jQuery(document).ready(function() { setTimeout(function() { window.location.replace("/"); }, 7000); });',
		  array('type' => 'inline', 'scope' => 'footer', 'weight' => 5)
		  );
  }
}

/**
 * Provides functionality for user thumbnails and the DSS departmental logo
 *
 */
function _bootstrap_dss_digital_user_logout($account) {

  if (variable_get('user_pictures', 0)) {

    if (!empty($account->picture)) {

      if (is_numeric($account->picture)) {

        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {

        $filepath = $account->picture->uri;
      }
    } elseif (variable_get('user_picture_default', '')) {

      $filepath = variable_get('user_picture_default', '');
    }

    if (isset($filepath)) {

      $alt = t("@user's picture", array('@user' => format_username($account)));
      // If the image does not have a valid Drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {

        $user_picture = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }
      else {

        $user_picture = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }

      /*
       * Generate the CAS logout link
       *
       */
      $attributes = array('https' => TRUE,
			  'attributes' => array('title' => t('Log out.')),
			  'html' => TRUE,
			  );

      // If we're currently authenticated by CAS, this apparently does not function...
      if(cas_user_is_logged_in()) {

        global $base_url;
        $attributes['query'] = array('destination' => current_path());
        return l($user_picture, 'caslogout', $attributes);
      }

      return l($user_picture, "user/logout", $attributes);
    }
  }
}

/**
 * Preprocess variables for page.tpl.php
 *
 * @see page.tpl.php
 */
function bootstrap_dss_digital_preprocess_page(&$variables) {

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 3;

  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['columns'] = 2;
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 2;
  }
  else {
    $variables['columns'] = 1;
  }

  // Primary nav
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  /**
   * browscap integration
   * Capture from the User-Agent value the type of device being used to browse the page
   * @todo (Probably should be decoupled and integrated into CSS and JavaScript)
   *
   */
  $browser = browscap_get_browser();
  $is_smartphone_browser = $browser['ismobiledevice'] && preg_match('/iPhone|(?:Android.*?Mobile)|(?:Windows Phone)/', $browser['useragent']);

  /**
   * Ensure that the "Contact Us" link directs users to the Drupal Node only for non-smartphone devices
   * Resolves DSSSM-635
   * @todo Refactor for specifying the path to the "Contact Us" form
   *
   */
  if($is_smartphone_browser) {

    // The "Contact Us" link (to the path "contact")
    $variables['contact_anchor'] = l(t('Contact Us'), 'contact');
  } else {

    // The "Contact Us" link
    $variables['contact_anchor'] = l(t('Contact Us'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
										      'data-target' => '#contact',
										      'data-anchor-align' => 'false'),
								'fragment' => ' ',
								'external' => TRUE));
  }

  // Different images must be passed based upon the browser type

  // Shouldn't be parsing the string itself; refactor
  if($is_smartphone_browser) {

    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/dss_logo_mobile.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  } else {

    // Work-around for the logo image
    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/dss_logo.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  }

  /**
   * Disabled for the initial release of the site
   * @todo Re-integrate for cases requiring Facebook and Twitter authentication
   *
   */
  //  $variables['auth_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#auth-modal" data-width-offset="0px" data-height-offset="30px"><div class="auth-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/UserIcon.png" /><span>Log In</span></div></a>';
  global $base_url;

  /**
   * Work-around for submitting GET parameters within the "destination" parameter for CAS redirection
   * Resolves DSS-192
   *
   */
  $GET_params = $_SERVER['QUERY_STRING'];

  $variables['auth_anchor'] = l('<div class="auth-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/UserIcon.png" /><span>Log In</span></div>',
				'cas',
				array('html' => TRUE,
				      'https' => true,
				      'query' => array('destination' => current_path() . '?' . $GET_params )
				      )
				);

  // The "Log Out" link
  // This needs to be refactored for integration with the CAS module
  // If we're currently authenticated by CAS, this apparently does not function...
  if(cas_user_is_logged_in()) {

    global $base_url;
    $variables['logout_anchor'] = l(t('Log Out'), 'caslogout', array('query' => array('destination' => current_path())));
  } else {

    $variables['logout_anchor'] = l(t('Log Out'), 'user/logout');
  }

  /**
   * Provide the share link
   * This integrates with the sharethis_helper.js
   *
   */
  $variables['share_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#share-modal" data-width-offset="10px" data-height-offset="28px"><div class="share-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/ShareIcon.png" /><span>Share</span></div></a>';

  // Render thumbnails for authenticated users
  $variables['user_picture'] = '<span class="button-auth-icon"></span>';

  if(user_is_logged_in()) {

    // For the user thumbnail
    global $user;

    $variables['user_picture'] = _bootstrap_dss_digital_user_logout($user);
  }

  /**
   * Variables for the Islandora simple_search Block
   *
   */
  // A search button must be passed if this is being viewed with a mobile browser

  $search_icon = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/SearchIcon.png',
				   'alt' => t('search the site'),
				   'attributes' => array()));

  /**
   * This was originally scoped for the insertion of a "simple search" block for smartphone devices
   * @todo Integrate for < 768 devices
   *
   */

  $simple_search_mobile = '<a data-toggle="lafayette-dss-modal" data-target="#advanced-search-modal" data-width-offset="-286px" data-height-offset="28px">
<div class="simple-search-icon">' . $search_icon . '<span>Search</span></div></a>' . render($variables['page']['simple_search']);
  unset($variables['page']['simple_search']);

  $variables['search_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';

  /**
   * @todo Restructure into a render array
   *
   */
  $auth_container = '<div class="auth-container modal-container container"><div id="auth-control-container" class="modal-control-container container">';

  /**
   * Insert the authentication link only if the user is anonymous
   *
   */

  if(!empty($variables['page']['auth'])) {

    $auth_container .= $variables['auth_anchor'];
  } else {

    $auth_container .= '
      <div class="auth-icon">' . $variables['user_picture'] . '</div>
      <div class="auth-link">' . $variables['logout_anchor'] . '</div>';
  }

  $auth_container .= '
       </div><!-- /#auth-control-container -->
     </div><!-- /.auth-container -->';

  $variables['auth_container'] = $auth_container;

  /**
   * @todo Refactor as a render array
   *
   */
  $share_container = '
     <div class="share-container modal-container container">
       <div id="share-control-container" class="modal-control-container container">

         ' . $variables['share_anchor'] . '
       </div><!-- /#share-control-container -->
     </div><!-- /.share-container -->';

  $variables['share_container'] = $share_container;

  /**
   * Structuring of the menu icon for the navbar
   *
   */
  $menu_toggle_image = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/MenuIcon.png',
                         'alt' => t('mobile menu'),
                         'attributes' => array()));

  $variables['menu_toggle_image'] = $menu_toggle_image;

  $menu_toggle_container = '

       <div id="menu-toggle-control-container" class="modal-control-container container">
<div class="navbar-collapse-toggle">
<!-- .btn-navbar is used as the toggle for collapsed navbar content -->
  <div data-toggle="collapse" data-target=".nav-collapse">
    <div id="menu-toggle-icon" class="navbar-icon btn-navbar">' . $menu_toggle_image . '<span id="btn-navbar-caption" class="">Menu</span></div>
  </div>
</div><!-- /.navbar-collapse-toggle -->
</div>';

  $variables['menu_toggle_container'] = $menu_toggle_container;

  /**
   * Integration of the Bootstrap carousel
   * @todo Refactor (possibly as a theme hook?)
   *
   */
  $variables['carousel'] = '

   <!-- Carousel -->
   <div id="carousel-featured-collection" class="carousel slide" data-ride="carousel">
    <!-- Indicators -->
    <ol class="carousel-indicators">
        <li data-target="#carousel-featured-collection" data-slide-to="0" class="active"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="1"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="2"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="3"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="4"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="5"></li>
    </ol>
    <!-- Wrapper for slides -->
    <div class="carousel-inner">
        <div class="item active">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselEAIC.jpg" alt="Detail of a Japanese postcard depicting the ceremony for rebuilding Ise Shrine, ca. 1918-31." />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/eastasia">East Asia Image Collection</a></p>
                <p class="carousel-caption-text"><a href="collections/eastasia/pa-koshitsu/ia0064">Japanese postcard depicting Ise Shrine rebuilding ceremony</a></p>
            </div>
        </div>
        <div class="item">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselELC.jpg" alt="1811 loan records for George Wolf, later Governor of Pennsylvania" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/eastonlibrary">Easton Library Company Database</a></p>
                <p class="carousel-caption-text"><a href="collections/eastonlibrary">1811 loan records for George Wolf, Governor of Pennsylvania</a></p>
            </div>
        </div>
        <div class="item">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselHistoric.jpg" alt="1896 portrait of football team on steps of Pardee Hall" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/historicalphotos">Historical Photograph Collection</a></p>
                <p class="carousel-caption-text"><a href="collections/historicalphotos/hpc-1155">Portrait of the Class of 1900 at the Senior Fence</a></p>
            </div>
        </div>
        <div class="item">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselNewspaper.jpg" alt="June 2, 1893 issue" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/newspaper">Lafayette Newspaper</a></p>
                <p class="carousel-caption-text"><a href="islandora/object/islandora:50527">June 2, 1893 issue</a></p>
            </div>
        </div>
        <div class="item">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselMarquis.jpg" alt="Predella scene from a lithograph portrait of Lafayette by Antoine Maurin (1797-1860)" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/lafayetteprints">Marquis de Lafayette Prints Collection</a></p>
                <p class="carousel-caption-text"><a href="collections/lafayetteprints/mdl-prints-0330">Scene from a portrait of Lafayette by Antoine Maurin</a></p>
            </div>
        </div>
        <div class="item">
            <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselSwift.jpg" alt="Opening lines of Baucis and Philemon, London, 1711" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="collections/spp">Swift Poems Project</a></p>
                <p class="carousel-caption-text"><a href="collections/spp">Opening lines of "Baucis and Philemon," London, 1711</a></p>
            </div>
        </div>
    </div>

    <!-- Controls --> <a class="left carousel-control" href="#carousel-featured-collection" data-slide="prev">
    <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselLeft.png" alt="carousel left nav button" />

   </a>  <a class="right carousel-control" href="#carousel-featured-collection" data-slide="next">
    <img src="/sites/all/themes/bootstrap_dss_digital/files/CarouselRight.png" alt="carousel right nav button" />
   </a>
</div>';

  /**
   * This was originally scoped for the insertion of a dynamic, sliding panel for the side (similar to what was offered for the Facebook mobile interface during 2013)
   * @todo Integrate and refactor into a theming hook
   *
   */
  $slide_panel_container = '';

  $variables['slide_panel_container'] = $slide_panel_container;

  $variables['breadcrumb'] = theme('breadcrumb', menu_get_active_trail());

  $variables['slide_drawers'] = TRUE;
}

/**
 * Implements template_preprocess_html
 *
 */
function bootstrap_dss_digital_preprocess_html(&$variables) {

  drupal_add_library('system', 'effects.drop');
  drupal_add_library('system', 'effects.slide');
}

/**
 * Implements template_process_page().
 *
 * Work-around for ensuring that the search form is not forcibly displayed within search results
 * See https://drupal.org/comment/4573218#comment-4573218
 * @todo Identify where this can be better handled and avoid this approach
 *
 */
function bootstrap_dss_digital_process_page(&$variables) {

  if(array_key_exists('search_form', $variables['page']['content']['system_main'])) {

    hide($variables['page']['content']['system_main']['search_form']);
  }
}

/**
 * Implements hook_theme_registry_alter().
 *
 */
function bootstrap_dss_digital_theme_registry_alter(&$registry) {

  $registry['hybridauth_widget']['file'] = 'template';
}

/**
 * Please see http://www.php.net/manual/en/function.ip2long.php#82397
 *
 * @todo Refactor into a more proper IP-address-based access control module
 * @todo Integrate with islandora_dss_solr_net_match()
 * @see islandora_dss_solr_net_match().
 *
 * This assumes a subnet of 139.147.0.0/16 for Lafayette College servers
 * This assumes a subnet of 192.168.101.0/24 for the VPN
 */
function bootstrap_dss_digital_net_match($CIDR, $IP) {

  list($net, $mask) = explode('/', $CIDR);
  return ( ip2long ($IP) & ~((1 << (32 - $mask)) - 1) ) == ip2long ($net);
}

function bootstrap_dss_digital_preprocess_islandora_large_image(array &$variables) {

  /**
   * Work-around given the issues for hook_menu_alter() and hook_preprocess_HOOK() implementations
   * @todo Refactor either into hook_menu_alter() or hook_preprocess_HOOK() implementations
   *
   */
  $object = $variables['islandora_object'];

  $client_ip = ip_address();
  $headers = apache_request_headers();

  // ...not within the campus network...
  // (for proxy servers...)
  if(array_key_exists('X-Forwarded-For', $headers)) {

    // Not on the VPN...
    $is_anon_non_lafayette_user = !islandora_dss_solr_net_match('192.168.101.0/24', $headers['X-Forwarded-For']);
    $is_anon_non_lafayette_user &= (bool) !islandora_dss_solr_net_match('139.147.0.0/16', $headers['X-Forwarded-For']);
  } else {

    // Not on the VPN...
    $is_anon_non_lafayette_user = !islandora_dss_solr_net_match('192.168.101.0/24', $client_ip);
    $is_anon_non_lafayette_user &= (bool) !islandora_dss_solr_net_match('139.147.0.0/16', $client_ip);
  }
  $is_anon_non_lafayette_user &= !user_is_logged_in(); // ...and not authenticated.

  // This fully resolves DSS-280
  $is_anon_non_lafayette_user = (bool) $is_anon_non_lafayette_user;

  if(in_array('islandora:geologySlidesEsi', $object->getParents()) and $is_anon_non_lafayette_user) {

    /**
     * Functionality for redirecting authentication requests over HTTPS
     * @see securelogin_secure_redirect()
     * @todo Refactor
     *
     */

    global $is_https;

    // POST requests are not redirected, to prevent unintentional redirects which
    // result in lost POST data. HTTPS requests are also not redirected.
    if(!$is_https) {

      $path = $_GET['q'];
      $http_response_code = 301;
      // Do not permit redirecting to an external URL.
      $options = array('query' => drupal_get_query_parameters(), 'https' => TRUE, 'external' => FALSE);
      // We don't use drupal_goto() here because we want to be able to use the
      // page cache, but let's pretend that we are.
      drupal_alter('drupal_goto', $path, $options, $http_response_code);
      // The 'Location' HTTP header must be absolute.
      $options['absolute'] = TRUE;
      $url = url($path, $options);
      $status = "$http_response_code Moved Permanently";
      drupal_add_http_header('Status', $status);
      drupal_add_http_header('Location', $url);
      // Drupal page cache requires a non-empty page body for some reason.
      print $status;
      // Mimic drupal_exit() and drupal_page_footer() and then exit.
      module_invoke_all('exit', $url);
      drupal_session_commit();
      if (variable_get('cache', 0) && ($cache = drupal_page_set_cache())) {

        drupal_serve_page_from_cache($cache);
      } else {

        ob_flush();
      }

      exit;
    } else {

      drupal_goto('cas', array('query' => array('destination' => current_path())));
    }
  }

  /**
   * Retrieve the label map from the MODS Document
   * @todo Refactor and decouple into a Collections/Solr labeling Module?
   *
   */
  try {

    $mods_str = $object['MODS']->content;

    $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);

    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {

    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $label_map = array_flip(islandora_solr_get_fields('result_fields', FALSE));

  /**
   * Resolves DSS-261
   *
   */
  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray($label_map) : array();

  $rendered_fields = array();
  foreach($variables['mods_object'] as $key => &$value) {

    if(!in_array($value['label'], $rendered_fields)) {

      $rendered_fields[] = $value['label'];
    } else {

      $value['label'] = '';
    }
  }

  /**
   * Work-around for appended site-generated resource metadata into the Object
   * Refactor (or, ideally, update the MODS when Drush creates or updates the path alias)
   * Resolves DSS-243
   *
   */

  global $base_url;
  $path_alias = 'http://digital.lafayette.edu/' . drupal_get_path_alias("islandora/object/{$object->id}");
  $variables['mods_object']['drupal_path'] = array('class' => '',
						   'label' => 'URL',
						   'value' => $path_alias,
						   'href' =>  $path_alias);
}

/**
 * Preprocessing for variables generated by the islandora_book_book() implementations
 * Implements hook_preprocess_HOOK().
 * @see islandora_book_preprocess_islandora_book_book().
 */
function bootstrap_dss_digital_preprocess_islandora_book_book(array &$variables) {

  $object = $variables['object'];

  /**
   * Work-around for displaying metadata
   * Refactor after re-indexing as transformed MODS
   * @todo Refactor and abstract for hook_islandora_large_image_preprocess().
   *
   */
  if(in_array('islandora:newspaper', $object->getParents())) {

    $mods_object = new DssDc($object['DC']->content);
  } else {

    // Refactor
    // Retrieve the MODS Metadata
    try {

      $mods_str = $object['MODS']->content;
      $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
      $mods_object = new DssMods($mods_str);
    } catch (Exception $e) {

      drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
    }
  }

  // Retrieve the labels from islandora_solr_get_fields()
  // Extended for additional fields
  $label_map = array_flip(islandora_solr_get_fields('result_fields', FALSE));

  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray($label_map) : array();

  $rendered_fields = array();
  foreach($variables['mods_object'] as $key => &$value) {

    if(!in_array($value['label'], $rendered_fields)) {

      $rendered_fields[] = $value['label'];
    } else {

      $value['label'] = '';
    }
  }

  /**
   * Work-around for appended site-generated resource metadata into the Object
   * Refactor (or, ideally, update the MODS when Drush creates or updates the path alias)
   * Resolves DSS-243
   *
   */

  global $base_url;
  $path_alias = 'http://digital.lafayette.edu/' . drupal_get_path_alias("islandora/object/{$object->id}");
  $variables['mods_object']['drupal_path'] = array('class' => '',
						   'label' => 'URL',
						   'value' => $path_alias,
						   'href' =>  $path_alias);
}

/**
 * Preprocessing for variables generated by the islandora_book_page() implementations
 * Implements hook_preprocess_HOOK().
 *
 */
function bootstrap_dss_digital_preprocess_islandora_book_page(array &$variables) {

  $object = $variables['object'];

  /**
   * Work-around for displaying metadata
   * Refactor after re-indexing as transformed MODS
   * @todo Refactor and abstract for hook_islandora_large_image_preprocess().
   *
   */
  try {

    $mods_str = $object['MODS']->content;
    $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {

    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray() : array();
}

function bootstrap_dss_digital_preprocess_islandora_book_pages(array &$variables) {

  // View Links.
  $display = (empty($_GET['display'])) ? 'grid' : $_GET['display'];
  $grid_active = ($display == 'grid') ? 'active' : '';
  $list_active = ($display == 'active') ? 'active' : '';

  $query_params = drupal_get_query_parameters($_GET);

  $variables['view_links'] = array(
    array(
      'title' => 'Grid view',
      'href' => url("islandora/object/{$object->id}/pages", array('absolute' => TRUE)),
      'attributes' => array(
        'class' => "islandora-view-grid $grid_active",
      ),
      'query' => $query_params + array('display' => 'grid'),
    ),
    array(
      'title' => 'List view',
      'href' => url("islandora/object/{$object->id}/pages", array('absolute' => TRUE)),
      'attributes' => array(
        'class' => "islandora-view-list $list_active",
      ),
      'query' => $query_params + array('display' => 'list'),
    ),
  );
}

/**
 * @todo Refactor in order to integrate with an administrative configuration interface
 *
 */
define('BOOTSTRAP_DSS_DIGITAL_BREADCRUMBS_MAX', 52);

function bootstrap_dss_digital_breadcrumb($variables) {

  /**
   * Work-around: attempt to retrieve the Object from vars
   * @todo Refactor (another theming hook?)
   *
   */
  if(array_key_exists(2, $variables)) {

    if(array_key_exists('map', $variables[2])) {

      if(array_key_exists(2, $variables[2]['map'])) {

        $object = $variables[2]['map'][2];
      }
    }
  }

  $output = '<ul class="breadcrumb">';

  /**
   * Work-around
   * @todo More properly integrate with core bootstrap theme functionality for breadcrumb generation
   * Note: Drupal Modules typically generate breadcrumbs simply using site Content Nodes and tokens (i. e. cannot integrate readily with Islandora)
   *
   */
  if(array_key_exists('breadcrumb', $variables)) {

    unset($variables['breadcrumb']);
  }

  $breadcrumbs = $variables;
  $count = count(array_keys($variables)) - 1;

  // For the truncation of individual breadcrumbs
  $breadcrumbs_length = 0;

  $path = current_path();
  $path_segments = explode('/', $path);

  $_breadcrumbs = $breadcrumbs;

  $searched_collection;
  $faceted_collection;

  /**
   * Unique to the islandora_solr_search Module
   * Attempt to build the breadcrumbs for Islandora Search results
   *
   */
  if(array_key_exists('q', $_GET)) {

    $solr_query = $_GET['q'];
    $facets = array();
    foreach($_GET as $param_key => $param_value) {

      if($param_key != 'q' && $param_key == 'f') {

        //$facets[] = array($param_key => $param_value);
        foreach($param_value as $facet) {

          $facet_split = explode(':', $facet);
          //$facet_field = $facet_split[0];
          $facet_field = array_shift($facet_split);
          $facet_value = implode(':', $facet_split);
          //$facets[$facet_field] = $facet_value;

          if(!array_key_exists($facet_field, $facets) and preg_match('/"(.+?)"/', $facet_value, $facet_value_match)) {

            $facets[$facet_field] = $facet_value_match[1];
          }
        }
      }
    }

    /**
     * @todo Abstract using variable_get()
     *
     */
    $eastasia_subcollections = array(
				     'Japanese Imperial House Postcard Album',
				     'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur',
				     'Imperial Postcard Collection',
				     'Tsubokura Russo-Japanese War Postcard Album',
				     'Sino-Japanese War Postcard Album 01',
				     'Sino-Japanese War Postcard Album 02',
				     'Lin Chia-Feng Family Postcard Collection',
				     'Japanese History Study Cards',
				     'Pacific War Postcard Collection',
				     'Michael Lewis Taiwan Postcard Collection',
				     'Gerald & Rella Warner Taiwan Postcard Collection',
				     'Gerald & Rella Warner Dutch East Indies Negative Collection',
				     'Japanese Imperial House Postcard Album',
				     'Gerald & Rella Warner Manchuria Negative Collection',
				     'Gerald & Rella Warner Taiwan Negative Collection',
				     'Gerald & Rella Warner Japan Slide Collection',
				     'Gerald & Rella Warner Souvenirs of Beijing and Tokyo',
				     'Woodsworth Taiwan Image Collection',
				     'Scenic Taiwan',
				     'Taiwan Photographic Monthly',
				     );

    /**
     * Work-around for linking Page Nodes to Islandora Collections
     * @todo Refactor for an admin. config. interface (Solr collection Content Node map)
     *
     */

    $collection_node_map = array(
				 'East Asia Image Collections' => 'node/26',
				 'East Asia Image Collection' => 'node/26',
				 'Easton Library Company' => 'node/30',
				 'Experimental Printmaking Institute Collection' => 'node/31',
				 'Geology Department Slide Collection' => 'node/19',
				 'Historical Photograph Collection' => 'node/20',
				 'Lafayette Newspaper Collection' => 'node/21',
				 'Marquis de Lafayette Prints Collection' => 'node/27',
				 'Silk Road Instrument Database' => 'node/32',
				 'Swift Poems Project' => 'node/33',
				 'Visual Resources Collection' => 'node/34',
				 'McKelvy House Photograph Collection' => 'node/42',
				 'Lafayette World War II Casualties' => 'node/43',
				 'Presidents of Lafayette College' => 'node/41',
				 );

    $collection_elements = array();

    /**
     * MODS-specific approach for generated nested collections
     * If one were to, instead, query against Solr, there would be an additional delay
     *
     */
    if(isset($object) and isset($object['MODS'])) {

      try {

        $mods_doc = new SimpleXMLElement($object['MODS']->content);
        $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
        $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

        /**
         * Just use the top-level collection element
         *
         */
        $collection_elements = array_merge($collection_elements, array(array('cdm.Relation.IsPartOf' => array_shift($mods_doc->xpath("./mods:note[@type='admin']")))));

        // Work-around for the Marquis de Lafayette Prints collection
        $map = function($element) {

          return array('mdl_prints.description.series' => $element);
        };
        $collection_elements = array_merge($collection_elements, array_map($map, $mods_doc->xpath("./mods:note[@type='series']")));

      } catch (Exception $e) {

        drupal_set_message(t('Error parsing the MODS metadata for the object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
      }

      unset($_breadcrumbs[count($_breadcrumbs) - 1]);
      $_breadcrumbs[count($breadcrumbs) - 2] = array('title' => 'Collections', 'href' => 'collections');

      //! @todo Abstract using variable_get()
      $map = function($element) {

        return array('cdm.Relation.IsPartOf' => $element);
      };

      /**
       * For building breadcrumbs from faceted search results from Solr
       *
       */
      if(!empty($collection_elements)) {

        $top_collection = (string) $collection_elements[0]['cdm.Relation.IsPartOf'];
        $_breadcrumbs[] = array('title' => $top_collection, 'href' => $collection_node_map[$top_collection]);
        $count++;

        $facet_params = array();

        $i=0;
        foreach($collection_elements as $collection_facet => $facets) {

          foreach($facets as $facet => $facet_value) {

            $facet_params["f[{$i}]"] = $facet . ':"' . $facet_value . '"';
            $i++;
          }
        }

        $_breadcrumbs[] = array('title' => 'Browse', 'href' => 'islandora/search/*:*', 'options' => array('query' => $facet_params));
        $count++;

      }

    } elseif(preg_match('/cdm\.Relation\.IsPartOf\:"(.+?)"/', $solr_query, $m)) { //! Unique to collections accessed using a specific field

      $title = $m[1];

      /**
       * Determine whether or not this belongs to the East Asia Image Collection Sub-Collection
       *
       */
      if(in_array($title, $eastasia_subcollections)) {

        $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/cdm.Relation.IsPartOf:"East Asia Image Collection"');
      } else {

        $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/cdm.Relation.IsPartOf:"East Asia Image Collection"');
      }

      $_breadcrumbs[] = array('title' => 'Search', 'href' => current_path());
      $count++;

    } else if(array_key_exists('mdl_prints.description.series', $facets)) { //! Determine whether or not this is a series of the Marquis de Lafayette Prints collection

      $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'Collections', 'href' => 'collections');
      $_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => $collection_node_map[$facets['cdm.Relation.IsPartOf']]);

      $_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"',
													      'f[1]' => 'mdl_prints.description.series:"' . $facets['mdl_prints.description.series'] . '"')));

      $count += 2;

    } else if(array_key_exists('cdm.Relation.IsPartOf', $facets)) { //! Determine whether or not this belongs to any collection with no nested Solr sub-collections

      $_breadcrumbs[count($breadcrumbs) - 1] = array('title' => 'Collections', 'href' => 'collections');

      // Hierarchical collections
      if(in_array($facets['cdm.Relation.IsPartOf'], $eastasia_subcollections)) {

        $_breadcrumbs[] = array('title' => "East Asia Image Collection", 'href' => '/islandora/search/' . $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"East Asia Image Collection"'
																			   )));
      } else {

        $_breadcrumbs[] = array('title' => $facets['cdm.Relation.IsPartOf'], 'href' => $collection_node_map[$facets['cdm.Relation.IsPartOf']]);
      }

      /**
       * Finally, provide the string "Browse" for the trailing breadcrumb element when browsing collections (as opposed to an explicit search
       *
       */
      $_breadcrumbs[] = array('title' => 'Browse', 'href' => $solr_query, 'options' => array('query' => array('f[0]' => 'cdm.Relation.IsPartOf:"' . $facets['cdm.Relation.IsPartOf'] . '"'
                                       )));
      $count += 2;

    } else { //! Handling Drupal site Content

      /**
       * @todo Integrate with the crumbs Module
       *
       */
      switch($solr_query) {

        case 'node/2':

          $_breadcrumbs[count($breadcrumbs) - 1]['title'] = t('Copyright & Use');
          break;

      case 'node/3':

        $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Services';
        break;

        case 'node/4':

          $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Repositories';
          break;

        case 'node/9':

          $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Contact DSS';
          break;

      case 'node/11':

        $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'People';
        break;

        case 'node/45':

          $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Collections';
          break;

        default:

          $_breadcrumbs[count($breadcrumbs) - 1]['title'] = 'Search';
          break;
      }
    }
  }

  /**
   * For generating breadcrumbs for apachesolr (i. e. site-level) searches
   *
   */
  if(isset($breadcrumbs[count($breadcrumbs) - 1])) {

    if(preg_match('/search\/node/', $breadcrumbs[count($breadcrumbs) - 1]['href'])) {

      /**
       * For apachesolr search queries
       * This resolves DSSSM-651
       *
       */
      $_breadcrumbs = array_slice($breadcrumbs, 0, 2);
      $count = 1;
    } else {

      switch($breadcrumbs[count($breadcrumbs) - 1]['href']) {

        /**
         * For those (rare) cases where Collection Objects are accessed *without* using a Solr facet
         * (These are likely legacy)
         * @todo Identify whether or not these can be removed
         *
         */
        case 'islandora/object/islandora:root':

          $_breadcrumbs = array($breadcrumbs[0], $breadcrumbs[count($breadcrumbs) - 1]);
          $count--;
          break;

        case 'islandora/object/islandora:eastAsia':
        case 'islandora/object/islandora:newspaper':
        case 'islandora/object/islandora:academicPublications':
        case 'islandora/object/islandora:administrativeArchive':
        case 'islandora/object/islandora:cap':
        case 'islandora/object/islandora:mdl':
        case 'islandora/object/islandora:geologySlidesEsi':
        case 'islandora/object/islandora:mckelvyHouse':
        case 'islandora/object/islandora:warCasualties':
        case 'islandora/object/islandora:presidents':

          $_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Digital Collections',
                      'href' => 'islandora/object/islandora:root')), array_slice($breadcrumbs, -1));
          $count++;
          break;

        case 'node/1':

	$_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1));
	$count--;
	break;

        case 'node/26':
        case 'node/30':
        case 'node/31':
        case 'node/19':
        case 'node/20':
        case 'node/21':
        case 'node/27':
        case 'node/32':
        case 'node/33':
        case 'node/34':
        case 'node/42':
        case 'node/43':

          /**
           * Site-specific logic which must be refactored
           * (These are pages for specific sub-collections)
           * @todo Identify how best to restructure this
           *
           */

          $_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Collections',
                              'href' => 'node/45')), array_slice($breadcrumbs, -1));
          $count++;
      break;

        case 'node/29':

          $_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Repositories',
                              'href' => 'node/4')), array_slice($breadcrumbs, -1));
          $count++;

          break;
      }
    }

    $breadcrumbs = $_breadcrumbs;

    $i = 1;

    /**
     * For the insertion of ellipses for the terminal breadcrumb element
     * @todo Invoke variable_get() in order to retrieve the maximum breadcrumbs
     *
     */
    foreach($breadcrumbs as $key => $breadcrumb) {

      if(isset($breadcrumb['href'])) {

        $breadcrumbs_length += strlen($breadcrumb['title']);

        if($breadcrumbs_length > BOOTSTRAP_DSS_DIGITAL_BREADCRUMBS_MAX) {

          if($key != count($breadcrumbs) - 1) {

            $breadcrumbs[$i]['title'] = '…';
            $breadcrumbs_length -= strlen($breadcrumb['title']) - 1;

            $i++;
          }
        }
      }
    }

    /**
     * For inserting a slash character ("/") between each breadcrumb
     *
     */
    foreach($breadcrumbs as $key => $breadcrumb) {

      if(isset($breadcrumb['href'])) {

        if(!isset($breadcrumb['options'])) {

          $breadcrumb['options'] = array();
        }

        if ($count != $key) {

          $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href'], $breadcrumb['options']) . '<span class="divider">/</span></li>';
        } else {

          $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href'], $breadcrumb['options']) . '</li>';
        }
      }
    }

    $output .= '</ul>';
    return $output;
  }
}
