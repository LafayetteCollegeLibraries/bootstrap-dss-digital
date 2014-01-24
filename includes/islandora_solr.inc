<?php

  /**
   * @file Functionality for the islandora_solr Module
   * @author griffinj@lafayette.edu
   *
   */

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

  dpm($variables);
}

function bootstrap_dss_digital_process_islandora_solr_wrapper(&$variables) {

  $display = (empty($_GET['display'])) ? variable_get('islandora_basic_collection_default_view', 'grid') : $_GET['display'];
  $variables['display'] = $display;

  $path = current_path();

  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List'),
    'attributes' => array(
      'href' => url($path, array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );

  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid'),
    'attributes' => array('href' => url($path, array('query' => $query_params)),
			  'class' => array('islandora-view-grid'),
			  ),
		     );
  
  $refine_link = array(
    'title' => t('Refine'),
    'attributes' => array('href' => '#',
			  'class' => array('snap-trigger')));

  // Refactor
  $view_icon_srcs = array('Refine' => base_path() . drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/RefineIcon.png',
			  'List' => base_path() . drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/ListIcon.png',
			  'Grid' => base_path() . drupal_get_path('theme', 'bootstrap_dss_digital') . '/files/GridIcon.png');

  $variables['view_icon_srcs'] = $view_icon_srcs;
  
  // Refactor
  $view_icon_alts = array('Refine' => 'refine search icon',
			  'List' => 'view list icon',
			  'Grid' => 'view grid icon');

  $variables['view_icon_alts'] = $view_icon_alts;

  // Refactor
  $view_icon_ids = array('Refine' => 'refine-search-icon',
			 'List' => 'view-list-icon',
			 'Grid' => 'view-grid-icon');

  $variables['view_icon_ids'] = $view_icon_ids;

  $variables['view_links'] = array($refine_link, $list_link, $grid_link);

  /*
  $results = $variables['results'];
v  $elements = $variables['elements'];
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


  */

  //drupal_set_title(drupal_get_title() . ' ' . $variables['islandora_solr_result_count']);

  dpm($variables);
}
