<?php

  /**
   * @file Functionality for islandora_basic_collection
   * @author griffinj@lafayette.edu
   *
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
  
  $variables['slide_drawers'] = TRUE;

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

    $grid_link = l(t('Grid view'),
		   $base_url . '/islandora/object/' . $islandora_object->id,
		   array('attributes' => array('class' => 'islandora-view-grid'),
			 'query' => $query_params));
  }

  $refine_link = l(t('Refine'),
		   $base_url . '/islandora/object/' . $islandora_object->id . '#',
		   array('attributes' => array('class' => array('snap-trigger'))));
  
  $variables['view_links'] = array('refine' => $refine_link,
				   'list' => $list_link,
				   'grid' => $grid_link);

  $variables['display'] = $display;

  dpm($variables);
}
