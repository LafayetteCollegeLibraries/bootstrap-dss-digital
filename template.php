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
    </ol>
    <!-- Wrapper for slides -->
    <div class="carousel-inner">
        <div class="item active">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselEAIC.jpg" alt="Geisha dances to samisen by Tamagawa near Tokyo" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/eastasia">East Asia Image Collections</a></p>
                <p class="carousel-caption-text"><a href="http://cdm.lafayette.edu/cdm4/item_viewer.php?CISOROOT=/eastasia&CISOPTR=1698&CISOBOX=1&REC=14">Japan Ministry of Justice Employees Labor Union March</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselELC.jpg" alt="1811 ledger of loan records for John Bowes" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/eastonlibrary">Easton Library Company Database</a></p>
                <p class="carousel-caption-text"><a href="projects/eastonlibrary">Ledger of loan records for Abraham Bachman</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselNewspaper.jpg" alt="May 11, 1956 issue" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/newspaper">Lafayette Newspaper</a></p> 
                <p class="carousel-caption-text"><a href="http://cdm.lafayette.edu/cdm4/document.php?CISOROOT=/newspaper&CISOPTR=9423&CISOSHOW=9417">May 11, 1956 issue</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselMarquis.jpg" alt="General Lafayette visiting George Washington at His Home" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/lafayetteprints">Marquis de Lafayette Prints Collection</a></p> 
                <p class="carousel-caption-text"><a href="http://cdm.lafayette.edu/cdm4/item_viewer.php?CISOROOT=/mdl-prints&CISOPTR=1970&CISOBOX=1&REC=8">General Lafayette visiting George Washington at His Home</a></p>
            </div>
        </div>
        <div class="item">
            <img src="sites/all/themes/bootstrap_dss_digital/files/CarouselHistoric.jpg" alt="1896 portrait of football team on steps of Pardee Hall" />
            <div class="carousel-caption">
                <p class="carousel-caption-heading"><a href="projects/historicalphotos">Historical Photograph Collection</a></p> 
                <p class="carousel-caption-text"><a href="http://cdm.lafayette.edu/cdm4/item_viewer.php?CISOROOT=/cap&CISOPTR=1180">1896 portrait of football team on steps of Pardee Hall</a></p>
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

  //dpm($variables['page']['content']);

  // Panel
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

  $variables['slide_panel_container'] = $slide_panel_container;

  $variables['breadcrumb'] = theme('breadcrumb', menu_get_active_trail());
  //$variables['breadcrumb'] = theme('breadcrumb', menu_get_active_breadcrumb());
  //$variables['breadcrumb'] = theme('breadcrumb', drupal_get_breadcrumb());
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

function bootstrap_dss_digital_process_islandora_basic_collection(&$variables) {

  $islandora_object = $variables['islandora_object'];
  $collection_pid = $islandora_object->id;

  //$slow_debug = FALSE;

  foreach($variables['associated_objects_array'] as &$associated_object) {

    $object = $associated_object['object'];
    $pid = $associated_object['pid'];

    /*
    if(!$slow_debug) {

      dpm($object['dc_array']);
      $slow_debug = TRUE;
    }
    */

    $title = $associated_object['title_link'];
    $thumbnail_img = $associated_object['thumbnail'];
    $object = $associated_object['object'];


    // Work-around
    // Refactor

    $pid_relation_is_part_of_map = array(
					 'eastAsia:imperialPostcards' => 'Imperial Postcard Collection',
					 'eastAsia:linPostcards' => 'Lin Chia-Feng Family Postcard Collection',
					 'eastAsia:lewis' => 'Michael Lewis Taiwan Postcard Collection',
					 'eastAsia:pacwarPostcards' => 'Pacific War Postcard Collection',
					 'eastAsia:paKoshitsu' => 'Japanese Imperial House Postcard Album',
					 'eastAsia:paOmitsu01' => 'Sino-Japanese War Postcard Album 01',
					 'eastAsia:paOmitsu02' => 'Sino-Japanese War Postcard Album 02',
					 'eastAsia:paTsubokura' => 'Tsubokura Russo-Japanese War Postcard Album',
					 );

    if(preg_match('/eastAsia:.*/', $pid)) {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"'. $pid_relation_is_part_of_map[$pid] .'"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"'. $pid_relation_is_part_of_map[$pid] .'"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:cap') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"Historical Photograph Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"Historical Photograph Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:geologySlidesEsi') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"John S. Shelton Earth Science Image Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"John S. Shelton Earth Science Image Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));

    } elseif($pid == 'islandora:mdlPrints') {

      $associated_object['title_link'] = l($title,
					   'islandora/search/cdm.Relation.IsPartOf:"Marquis de Lafayette Prints Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    
      $associated_object['thumb_link'] = l($thumbnail_img,
					   'islandora/search/cdm.Relation.IsPartOf:"Marquis de Lafayette Prints Collection"',
					   array('html' => TRUE,
						 'alias' => TRUE,
						 'attributes' => array('title' => $title)));
    }
  }
}

function bootstrap_dss_digital_preprocess_islandora_basic_collection_wrapper(&$variables) {

  // For rendering non-grid content
  drupal_add_css(drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.base.css');
  drupal_add_css(drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.theme.css');

  /*
  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );

  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-grid'),
    ),
  );
  */
  $islandora_object = $variables['islandora_object'];
  $display = (empty($_GET['display'])) ? variable_get('islandora_basic_collection_default_view', 'grid') : $_GET['display'];
  $link_text = (empty($_GET['display'])) ? 'grid' : $_GET['display'];
  $query_params = drupal_get_query_parameters($_GET);

  global $base_url;

  if ($display == 'grid') {

    $query_params['display'] = 'list';
    
    /*
    $list_link = array(
		       'title' => 'List view',
		       'href' => $base_url . '/islandora/object/' . $islandora_object->id,
		       'attributes' => array(
					     'class' => 'islandora-view-list',
					     ),
		       'query' => $query_params,
		       );
    */

    $list_link = l('List view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-list')),
			 'query' => $query_params));

    unset($query_params['display']);
    $query_params['display'] = 'grid';

    /*
    $grid_link = array(
      'title' => 'Grid view',
      'href' => $base_url . '/islandora/object/' . $islandora_object->id,
      'attributes' => array('class' => array('islandora-view-grid', 'active')),
      'query' => $query_params,
    );
    */

    $grid_link = l('Grid view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-grid', 'active')),
			 'query' => $query_params));

  } else {

    $query_params['display'] = 'list';

    /*
    $list_link = array(
      'title' => 'List view',
      'href' => $base_url . '/islandora/object/' . $islandora_object->id,
      'attributes' => array('class' => array('islandora-view-list', 'active')),
      'query' => $query_params,
    );
    */

    $list_link = l('List view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => array('islandora-view-list', 'active')),
			 'query' => $query_params));

    unset($query_params['display']);
    $query_params['display'] = 'grid';

    /*
      $grid_link = array(
		       'title' => 'Grid view',
		       'href' => $base_url . '/islandora/object/' . $islandora_object->id,
		       'attributes' => array('class' => 'islandora-view-grid'),
		       'query' => $query_params,
		       );
    */

    $grid_link = l('Grid view',
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => 'islandora-view-grid'),
			 'query' => $query_params));
  }

  $variables['view_links'] = array($list_link, $grid_link);
}

function bootstrap_dss_digital_theme_registry_alter(&$registry) {

  $registry['hybridauth_widget']['file'] = 'template';

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

  $breadcrumbs = $_breadcrumbs;

  foreach($breadcrumbs as $key => $breadcrumb) {

    if ($count != $key) {

      $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href']) . '<span class="divider">/</span></li>';
    } else {

      $output .= '<li>' . l($breadcrumb['title'], $breadcrumb['href']) . '</li>';
    }
  }

  $output .= '</ul>';
  return $output;
}

function bootstrap_dss_digital_process_islandora_solr_wrapper(&$variables) {

  $path = current_path();

  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );

  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-grid'),
    ),
  );

  $variables['view_links'] = array($list_link, $grid_link);

  /*
  $results = $variables['results'];
  $elements = $variables['elements'];
  $pids = $variables['pids'];

  //dpm($results);
  //dpm($elements);

  if ($display == 'grid') {

    $grid_link['attributes']['class'][] = 'active';
    $content = theme('islandora_solr', array(
					     'results' => $results,
					     'elements' => $elements,
					     //'pids' => $pids
					     ));
  } else {

    $list_link['attributes']['class'][] = 'active';
    $content = theme('islandora_solr', array(
					     'results' => $results,
					     'elements' => $elements,
					     //'pids' => $pids
					     ));
  }

  $variables['content'] = $content;

  dpm($variables);
  */

}

/**
 * Implements hook_preprocess_theme()
 * @see islandora_solr_islandora_solr
 *
 */

function bootstrap_dss_digital_preprocess_islandora_solr(&$variables) {

  $display = (empty($_GET['display'])) ? 'list' : $_GET['display'];

  $path = current_path();

  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );

  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid view'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-grid'),
    ),
  );

  /*
  if($display == 'grid') {

    $variables['theme_hook_suggestions'][] = 'islandora_solr_grid';
  }
  */

  $variables['display'] = $display;
  dpm($variables);

  //$variables['view_links'] = array($grid_link, $list_link);
  //$islandora_object = $variables['islandora_object'];


  //dpm(array_keys($variables));
  //dpm($variables['results']);

  if(preg_match('/cdm\.Relation\.IsPartOf\:"(.+?)"/', current_path(), $m)) {

    $relation = $m[1];

    $relation_is_part_of_dc_field_map = array(
					      'Marquis de Lafayette Prints Collection' => array(
												'dc.description',
												'dc.format',
												'dc.identifier',
												'dc.rights',
												'dc.subject',
												'dc.type'
												),
					      'John S. Shelton Earth Science Image Collection' => array('dc.contributor',
													'dc.coverage',
													'dc.description',
													'dc.format',
													'dc.identifier',
													'dc.language',
													'dc.publisher',
													'dc.subject',
													'dc.type',
													)
					      );

    /*
    $relation_is_part_of_dc_field_label_map = array('Marquis de Lafayette Prints Collection' => array(
												      'dc.description' => '',
												      'dc.identifier' => ''
												      ));
    */

    foreach($variables['results'] as &$result) {
      
      foreach($result['solr_doc'] as $field_name => &$field) {

	if(in_array($relation, $relation_is_part_of_dc_field_map)) {

	    if(in_array($field_name, $relation_is_part_of_dc_field_map[$relation])) {
	  
	      unset($result['solr_doc'][$field_name]);
	    }
	}

	/*
	if(in_array($field_name, $relation_is_part_of_dc_field_map[$relation])) {

	  $result['solr_doc'][$field_name]['label'] = $relation_is_part_of_dc_field_label_map[$relation][$field_name];
	}
	*/
      }
    }
  }

  // For rendering non-grid content
  drupal_add_css(drupal_get_path('module', 'islandora_basic_collection') . '/css/islandora_basic_collection.base.css');
  drupal_add_css(drupal_get_path('module', 'islandora_basic_collection') . '/css/islandora_basic_collection.theme.css');

}
