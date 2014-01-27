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
require_once dirname(__FILE__) . '/includes/pager.inc';
require_once dirname(__FILE__) . '/includes/islandora_solr.inc';
require_once dirname(__FILE__) . '/includes/islandora_basic_collection.inc';



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

  // Different images must be passed based upon the browser type

  // Shouldn't be parsing the string itself; refactor
  if($is_smartphone_browser) {
    //if(TRUE) {

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

  $variables['auth_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#auth-modal" data-width-offset="0px" data-height-offset="30px"><div class="auth-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/UserIcon.png" /><span>Log In</span></div></a>';

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

  $variables['share_anchor'] = '<a data-toggle="lafayette-dss-modal" data-target="#share-modal" data-width-offset="10px" data-height-offset="28px"><div class="share-icon navbar-icon"><img src="/sites/all/themes/bootstrap_dss_digital/files/ShareIcon.png" /><span>Share</span></div></a>';

  // Render thumbnails for authenticated users
  $variables['user_picture'] = '<span class="button-auth-icon"></span>';

  if(user_is_logged_in()) {

    // For the user thumbnail
    global $user;
    $user_view = user_view($user);
    $variables['user_picture'] = drupal_render($user_view['user_picture']);
  }

  // A search button must be passed if this is being viewed with a mobile browser

  $search_icon = theme_image(array('path' => drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/SearchIcon.png',
				   'alt' => t('search the site'),
				   'attributes' => array()));

  $simple_search_mobile = '<a data-toggle="lafayette-dss-modal" data-target="#advanced-search-modal" data-width-offset="-286px" data-height-offset="28px">
<div class="simple-search-icon">' . $search_icon . '<span>Search</span></div></a>' . render($variables['page']['simple_search']);
  unset($variables['page']['simple_search']);
  //$variables['simple_share_mobile_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';
  $variables['search_container'] = '<div class="modal-container container"><div id="simple-search-control-container" class="modal-control-container container">' . $simple_search_mobile . '</div></div>';


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

  // Carousel
  $variables['carousel'] = '

   <!-- Carousel -->
<div id="carousel-featured-collection" class="carousel slide" data-ride="carousel" >
    <!-- Indicators -->
    <ol class="carousel-indicators">
        <li data-target="#carousel-featured-collection" data-slide-to="0" class="active"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="1"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="2"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="3"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="4"></li>
        <li data-target="#carousel-featured-collection" data-slide-to="6"></li>
    </ol>
    <!-- Wrapper for slides -->
    <div class="carousel-inner">
        <div class="item active">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselEAIC.jpg" alt="Detail of a Japanese postcard depicting the ceremony for rebuilding Ise Shrine, ca. 1918-31." />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/eastasia">East Asia Image Collections</a></p>
                <p class="carousel-caption-text"><a href="http://digital.stage.lafayette.edu/islandora/object/islandora%3A33684">Japanese postcard depicting Ise Shrine rebuilding ceremony</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselELC.jpg" alt="1811 loan records for George Wolf, later Governor of Pennsylvania" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/eastonlibrary">Easton Library Company Database</a></p>
                <p class="carousel-caption-text"><a href="projects/eastonlibrary">1811 loan records for George Wolf, Governor of Pennsylvania</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselHistoric.jpg" alt="1896 portrait of football team on steps of Pardee Hall" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/historicalphotos">Historical Photograph Collection</a></p> 
                <p class="carousel-caption-text"><a href="projects/historicalphotos">Portrait of the Class of 1900 at the Senior Fence</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselNewspaper.jpg" alt="June 2, 1893 issue" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/newspaper">Lafayette Newspaper</a></p> 
                <p class="carousel-caption-text"><a href="projects/newspaper">June 2, 1893 issue</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselMarquis.jpg" alt="Predella scene from a lithograph portrait of Lafayette by Antoine Maurin (1797-1860)" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/lafayetteprints">Marquis de Lafayette Prints Collection</a></p> 
                <p class="carousel-caption-text"><a href="projects/lafayetteprints">Scene from a portrait of Lafayette by Antoine Maurin</a></p>
            </div>
        </div>                         
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselSwift.jpg" alt="Opening of Excellent New Panegyrick on Skinnibonia by Jonathan Swift, with Numbr 2. written in margin by Swift himself" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/spa">Swift Poems Project</a></p>
                <p class="carousel-caption-text"><a href="projects/spa">"Numbr 2." written in margin by Swift on one of his poems</a></p>
            </div>
        </div>
    </div>
    <!-- Controls --> <a class="left carousel-control" href="#carousel-featured-collection" data-slide="prev">                                                                                                                 
    <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselLeft.png" alt="carousel left nav button" />
   </a>  <a class="right carousel-control" href="#carousel-featured-collection" data-slide="next">                                                                                                                
    <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselRight.png" alt="carousel right nav button" />
   </a> 
</div>';
    
  // Adding the tabs for certain nodes
  /*
  $eastasia_tabs = quicktabs_load('east_asia_image_collections');
  $mdl_tabs = quicktabs_load('marquis_de_lafayette_prints_coll');

  $variables['tabs'] = array('eastasia_tabs' => theme('quicktabs', (array) $eastasia_tabs),
			     'mdl_tabs' => theme('quicktabs', (array) $mdl_tabs));
  */



  // Panel
  /*
  $slide_panel_container = '
      <div id="menu" class="menu nav-collapse collapse width">
        <div class="collapse-inner">
          <div class="navbar navbar-inverse">
            <div class="navbar-inner">
              Menu
            </div>
          </div>
        ' . $variables['page']['slide_panel'] . '
        </div>
      </div><!-- /#menu -->
      <div class="view">
        <div class="navbar navbar-inverse">
          <div class="navbar-inner">
            <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target="#menu">
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
            </button>
          </div>
        </div><!-- /.view -->
      </div>
';
  */
  $slide_panel_container = '';

  $variables['slide_panel_container'] = $slide_panel_container;

  $variables['breadcrumb'] = theme('breadcrumb', menu_get_active_trail());
  //$variables['breadcrumb'] = theme('breadcrumb', menu_get_active_breadcrumb());
  //$variables['breadcrumb'] = theme('breadcrumb', drupal_get_breadcrumb());

  $variables['slide_drawers'] = TRUE;
}

/**
 * Implements template_preprocess_html
 *
 */
function bootstrap_dss_digital_preprocess_html(&$variables) {

  drupal_add_library('system', 'effects.drop');
  drupal_add_library('system', 'effects.slide');

  //$variables['slide_panel'] = $variables['page']['slide_panel'];
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
  
  //dpm($registry['pager']);

  // Work-around
  //$registry['islandora_basic_collection_wrapper']['preprocess functions'] = array('bootstrap_dss_digital_preprocess_islandora_basic_collection');

  /*
    'islandora_basic_collection_wrapper' => array(
      'file' => 'theme/theme.inc',
      'template' => 'theme/islandora-basic-collection-wrapper',
      'variables' => array('islandora_object' => NULL),
  */
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

    //dpm($mods_str);

    $mods_object = new DssMods($mods_str);
  } catch (Exception $e) {
    
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['mods_object'] = isset($mods_object) ? $mods_object->toArray() : array();
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

function bootstrap_dss_digital_breadcrumb($variables) {

  $output = '<ul class="breadcrumb">';

  // Work-around
  if(array_key_exists('breadcrumb', $variables)) {

    unset($variables['breadcrumb']);
  }

  $breadcrumbs = $variables;
  $count = count(array_keys($variables)) - 1;

  $path = current_path();
  $path_segments = explode('/', $path);

  $_breadcrumbs = $breadcrumbs;

  if(isset($breadcrumbs[count($breadcrumbs) - 1])) {
    switch($breadcrumbs[count($breadcrumbs) - 1]['href']) {

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

      $_breadcrumbs = array_merge(array_slice($breadcrumbs, 0, -1), array(array('title' => 'Projects',
									      'href' => 'node/12')), array_slice($breadcrumbs, -1));
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

  foreach($breadcrumbs as $key => $breadcrumb) {

    if(isset($breadcrumb['href'])) {

      if ($count != $key) {

	$output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href']) . '<span class="divider">/</span></li>';
      } else {

	$output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href']) . '</li>';
      }
    }
  }

  $output .= '</ul>';
  return $output;
}
