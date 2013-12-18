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
require_once dirname(__FILE__) . '/includes/dssMods.inc';

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

  // The "Contact Us" link
  $variables['contact_anchor'] = l(t('Contact Us'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
										    'data-target' => '#contact',
										    'data-anchor-align' => 'false'),
							      'fragment' => ' ',
							      'external' => TRUE));

  $browser = browscap_get_browser();
  $is_smartphone_browser = $browser['ismobiledevice'] && preg_match('/iPhone|(?:Android.*?Mobile)|(?:Windows Phone)/', $browser['useragent']);

  dpm($browser);

  // Different images must be passed based upon the browser type

  // Shouldn't be parsing the string itself; refactor
  //if($is_smartphone_browser) {
  if(TRUE) {

    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/dss_logo_mobile.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  } else {

    // Work-around for the logo image
    $variables['dss_logo_image'] = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/dss_logo.png',
						     'alt' => t('digital scholarship services logo'),
						     'attributes' => array()));
  }

  // The "Log In" link
  //$variables['auth_anchor'] = l(t('Log In'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
  /*
  $variables['auth_anchor'] = l('<div class="auth-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/UserIcon.png" /><span>Log In</span></div>', '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
														    'data-target' => '#auth-modal',
																								  'data-width-offset' => '10px',
														    'data-height-offset' => '28px'),
											      'fragment' => ' ',
											      //'external' => TRUE));
											      'external' => TRUE,
											      'html' => TRUE
											      ));
  */

  $variables['auth_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#auth-modal" data-width-offset="0px" data-height-offset="30px"><div class="auth-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/UserIcon.png" /><span>Log In</span></div></a>';

  // The "Log Out" link
  $variables['logout_anchor'] = l(t('Log Out'), 'user/logout');

  // The "Share" link
  //$variables['share_anchor'] = l(t('Share'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
  /*
  $variables['share_anchor'] = l('<div class="share-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/ShareIcon.png" /><span>Share</span></div>', '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
									     'data-target' => '#share-modal',
																								    'data-width-offset' => '10px',
									     'data-height-offset' => '28px'
									     ),
						       'fragment' => ' ',
						       //'external' => TRUE));
						       'external' => TRUE,
						       'html' => TRUE
						       ));
  */

  $variables['share_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#share-modal" data-width-offset="10px" data-height-offset="28px"><div class="share-icon"><img src="/sites/all/themes/bootstrap_lafayette_lib_dss/files/ShareIcon.png" /><span>Share</span></div></a>';

  // Render thumbnails for authenticated users
  // By default, use a glyphicon
  $variables['user_picture'] = '<span class="button-auth-icon"></span>';

  if(user_is_logged_in()) {

    // For the user thumbnail
    global $user;
    $user_view = user_view($user);
    $variables['user_picture'] = drupal_render($user_view['user_picture']);
  }

  // Ensure that "home" always exists for the breadcrumbs
  if(empty($variables['breadcrumb'])) {

    $variables['breadcrumb'] = '<ul class="breadcrumb"><li>' . l(t('Home'), $variables['front_page']) . '</li></ul>';
  }

  // A search button must be passed if this is being viewed with a mobile browser
  //if($is_smartphone_browser) {
  // Dev
  if(TRUE) {

    $simple_search_mobile = '<a><div class="simple-search-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/simple_search_mobile_icon.png" /><span>Search</span></div></a>' . render($variables['page']['simple_search']);
    unset($variables['page']['simple_search']);
    $variables['simple_share_mobile_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';
  }
  
  // Refactor
  $auth_container = '
     <div class="auth-container modal-container container">
       <div id="auth-control-container" class="modal-control-container container">';

  /*
    <?php if (!empty($page['auth'])): ?>

    <!-- <div class="auth-icon"><img src="/sites/all/themes/bootstrap_dss_islandora_dev/files/UserIcon.png" /></div> -->
    <?php print $auth_anchor; ?>
    <?php else: ?>
    
    <div class="auth-icon"><?php print $user_picture; ?></div>
    <div class="auth-link"><?php print $logout_anchor; ?></div>
    <?php endif; ?>
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

  $share_container = '
     <div class="share-container modal-container container">
       <div id="share-control-container" class="modal-control-container container">

         ' . $variables['share_anchor'] . '
       </div><!-- /#share-control-container -->
     </div><!-- /.share-container -->';

  $variables['share_container'] = $share_container;

  // Carousel
  $variables['carousel'] = '

   <!-- Carousel -->
   <div id="carousel-featured-collection" class="carousel slide" data-ride="carousel">

   <!-- Indicators -->
   <ol class="carousel-indicators">
     <li data-target="#carousel-featured-collection" data-slide-to="0" class="active"></li>
     <li data-target="#carousel-featured-collection" data-slide-to="1"></li>
     <li data-target="#carousel-featured-collection" data-slide-to="2"></li>
   </ol>

  <!-- Wrapper for slides -->
  <div class="carousel-inner">
    <div class="item active">
      <img src="sites/all/themes/bootstrap_dss_digital/files/carousel_test_1400_240.png" alt="Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921">
      <div class="carousel-caption">
        <h2>East Asia Image Collections</h2>
          <p>Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921</p>
      </div>
    </div>
    <div class="item">
      <img src="sites/all/themes/bootstrap_dss_digital/files/carousel_test_1400_240.png" alt="Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921">
      <div class="carousel-caption">
        <h2>East Asia Image Collections</h2>
        <p>Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921</p>
      </div>
    </div>
    <div class="item">
      <img src="sites/all/themes/bootstrap_dss_digital/files/carousel_test_1400_240.png" alt="Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921">
      <div class="carousel-caption">
        <h2>East Asia Image Collections</h2>
        <p>Crown Prince Hirohito in England at Tomb of Unknown Soldier, 1921</p>
      </div>
    </div>
   </div>

   <!-- Controls -->
   <a class="left carousel-control" href="#carousel-featured-collection" data-slide="prev">
     <span class="glyphicon glyphicon-chevron-left"></span>
   </a>
   <a class="right carousel-control" href="#carousel-featured-collection" data-slide="next">
     <span class="glyphicon glyphicon-chevron-right"></span>
   </a>
   </div>';
    


}

/**
 * Implements template_preprocess_hybridauth_widget
 * @griffinj
 *
 */
function bootstrap_dss_digital_preprocess_hybridauth_widget(&$vars) {

  // Refactor
  $i = 0;
  foreach (hybridauth_get_enabled_providers() as $provider_id => $provider_name) {

    //$vars['providers'][$i] = preg_replace('/(<\/span>)/', "</span><span>&nbsp;$provider_name</span>", $vars['providers'][$i]);
    $i++;
  }
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
 * Template preprocess function for hybridauth_widget.
 */
/*
function template_preprocess_hybridauth_widget(&$vars, $hook) {

}
*/

function bootstrap_dss_digital_theme_registry_alter(&$registry) {

  $registry['hybridauth_widget']['file'] = 'template';
}

/**
 * Implements hook_theme().
 */
/*
function hybridauth_theme($existing, $type, $theme, $path) {
  return array(
    'hybridauth_admin_settings_providers_table' => array(
      'render element' => 'form',
      'file' => 'hybridauth.admin.inc',
    ),
    'hybridauth_widget' => array(
      'render element' => 'element',
      'template' => 'templates/hybridauth_widget',
      'file' => 'hybridauth.theme.inc',
    ),
}
*/

//module_load_include('inc', 'bootstrap_dss_digital', 'includes/dssMods');

function bootstrap_dss_digital_preprocess_islandora_book_book(array &$variables) {

  $object = $variables['object'];

  // Refactor
  // Retrieve the MODS Metadata
  try {

    $mods_str = $object['MODS']->content;

    $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
    //$mods_str = '<modsCollection>' . $mods_str . '</modsCollection>';

    dpm($mods_str);

    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {
    
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['mods_object'] = isset($mods_object) ? $mods_object->mods : array();
}

function bootstrap_dss_digital_preprocess_islandora_book_page(array &$variables) {

  $object = $variables['object'];

  // Refactor
  // Retrieve the MODS Metadata
  try {

    $mods_str = $object['MODS']->content;

    $mods_str = preg_replace('/<\?xml .*?\?>/', '', $mods_str);
    //$mods_str = '<modsCollection>' . $mods_str . '</modsCollection>';

    //dpm($mods_str);

    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {
    
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['mods_object'] = isset($mods_object) ? $mods_object->mods : array();
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

  dpm($variables);
}

