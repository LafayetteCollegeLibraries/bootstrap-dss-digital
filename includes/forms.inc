<?php

  /**
   * @file
   * Theming for forms
   *
   */

  /**
   * Islandora Solr advanced search block form
   * @author griffinj
   *
   * @see islandora_solr_advanced_search_form($form, &$form_state)
   *
   * @global type $_islandora_solr_queryclass
   *   the IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   * @param type $form
   * @param array $form_state
   * @return string
   */

function bootstrap_dss_digital_islandora_solr_advanced_search_form($form, &$form_state) {
  drupal_add_library('system', 'ui.tabs');

  global $_islandora_solr_queryclass;

  // include common.inc
  module_load_include('inc', 'islandora_solr', 'includes/common');

  // check form states
  // 1. form update using ajax
  // 2. populate with current query on search results page
  // 3. anywhere else: empty form

  // 1. form update using ajax
  if (isset($form_state['triggering_element'])) {

    // unset terms from input array
    // http://drupal.stackexchange.com/questions/14855/how-do-i-dynamically-fill-a-textfield-with-ajax/16576#16576
    unset($form_state['input']['terms']);

    /**
     * @author griffinj@lafayette.edu
     *
     */
    // check for add
    if ($form_state['triggering_element']['#value'] == 'Add a field') {
      $form_state['values']['terms'][] = array();
    }
    // check for remove
    elseif ($form_state['triggering_element']['#value'] == 'Remove this field') {
      $field = $form_state['triggering_element']['#field'];
      array_splice($form_state['values']['terms'], $field, 1);
    }
    // set values
    $values = $form_state['values'];
  } elseif (islandora_solr_results_page($_islandora_solr_queryclass) == TRUE && !isset($_islandora_solr_queryclass->internalSolrParams['type'])) {

    // 2. populate with current query on search results page

    // get current query
    $query = $_islandora_solr_queryclass->solrQuery;
    // set value variable
    $values['terms'] = array();

    // explode on space
    $query_explode = explode(' ', $query);

    // break up the solr query to populate the advanced search form
    $i = 0;
    foreach ($query_explode as $key => $value) {
      $term = array();

      // check for first colon to split the string
      if (strpos($value, ':') != FALSE) {
        // split the filter into field and value
        $value_split = explode(':', $value, 2);
        // set field
        $values['terms'][$i]['field'] = $value_split[0];

        // second part of the split is the query value (or first part of it)
        $value_split[1] = str_replace(array('(', ')'), '', $value_split[1]);
        // add search string
        $values['terms'][$i]['search'] = $value_split[1];

      }
      // if the string does not include a colon or AND/OR/NOT, then it is a
      // part of the query value
      elseif (!preg_match('/(AND|OR|NOT)/', $value, $matches)) {
        // trim brackets
        $value = str_replace(array('(', ')'), '', $value);

        if (isset($values['terms'][$i]['search'])) {
          // append to search string
          $values['terms'][$i]['search'] .= ' ' . $value;
        }
        else {
          // search field is not set, so create new search value
          $values['terms'][$i]['search'] = $value;
        }
      }
      // if it matches AND/OR/NOT, then we have the boolean operator
      else {
        // set boolean operator
        $values['terms'][$i]['boolean'] = $value;

        // increment to next field
        $i++;
      }
    }
  }
  // 3. anywhere else: empty form
  else {
    //Need at least one term to draw the search box.
    $values = array(
      'terms' => array(''),
    );
  }

  $terms = array(
    '#type' => 'markup',
    '#prefix' => '<div id="islandora-solr-advanced-terms">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  // loop over each term
  foreach ($values['terms'] as $i => $value) {

    $term = array('#tree' => TRUE,
		  '#prefix' => '<div>',
		  '#suffix' => '</div>');

    $term['field'] = array(
      '#title' => t('Field'),
      '#type' => 'select',
      '#default_value' => isset($value['field']) ? $value['field'] : 'dc.title',
      '#options' => islandora_solr_get_fields('search_fields'),
      '#attributes' => array('class' => array('islandora-solr-search-field')));

    $term['search'] = array(
			    //'#title' => t('Search Terms'),
			    '#type' => 'textfield',
			    '#size' => 20,
			    '#default_value' => isset($value['search']) ? $value['search'] : 'Search...',
			    );

    $term['hidden_submit'] = array(// Used for when the user presses enter on the search field.
      '#type' => 'submit',
      '#value' => t('Search'),
      '#attributes' => array('style' => 'visibility:hidden;position:fixed;top:-1000px;right:-1000px;')
    );

    /**
     * @author griffinj@lafayette.edu
     * Provide buttons for adding additional search fields based upon the number of terms within the form
     *
     */

    if(count($values['terms']) < 5) {

      $term['add'] = array(
			   '#type' => 'button',
			   '#value' => 'Add a field',
			   '#attributes' => array('title' => t('Add field'),
						  'class' => array('islandora-solr-search-add')),
			   '#name' => 'add-field-' . $i,
			   '#ajax' => array(
					    'callback' => '_islandora_solr_advanced_search_terms',
					    'wrapper' => 'islandora-solr-advanced-terms',
					    'method' => 'replace',
					    'effect' => 'fade',
					    'progress' => array('type' => 'none'),
					    ),
			   );
    }
    if (count($values['terms']) > 1) {
      $term['remove'] = array(
        '#type' => 'button',
        '#field' => $i,
        '#value' => 'Remove this field',
        '#attributes' => array('title' => t('Remove field'),
			       'class' => array('islandora-solr-search-remove')),
        '#name' => 'remove-field-' . $i,
        '#ajax' => array(
          'callback' => '_islandora_solr_advanced_search_terms',
          'wrapper' => 'islandora-solr-advanced-terms',
          'method' => 'replace',
          'effect' => 'fade',
          'progress' => array('type' => 'none'),
        )
      );

      if ((variable_get('islandora_solr_search_boolean', 'user') == 'user') && ((count($values['terms']) -1) != $i)) {
        $term['boolean'] = array(
          '#type' => 'select',
          '#prefix' => '<div class="islandora-solr-advanced-boolean">',
          '#suffix' => '</div>',
          '#default_value' => isset($value['boolean']) ? $value['boolean'] : 'AND',
          '#options' => array(
            'AND' => 'AND',
            'OR' => 'OR',
            'NOT' => 'NOT'),
	  '#attributes' => array('class' => array('islandora-solr-search-boolean'))
        );
      }
    }
    $terms[] = $term;
  }

  // set form
  $form = array();
  // add terms
  $form['terms'] = $terms;

  // add controls
  $form['controls'] = array('#type' => 'markup',
			    '#prefix' => '<div class="islandora-solr-advanced-controls">',
			    '#suffix' => '</div>');

  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search')
  );
  return $form;
}

/**
 * griffinj
 * Implements hook_form_alter()
 *
 * Theming for the advanced search form must be implemented using a hook which does not conflict with the actual form implementation within the global context
 *
 */
function bootstrap_dss_digital_form_alter(&$form, $form_state, $form_id) {

  switch($form_id) {

  case 'islandora_dss_solr_advanced_search_form':

    foreach($form['terms'] as $key => &$term) {

      if(is_numeric($key)) {

	$term['field-wrapper']['search']['#size'] = 40;
      }
    }
    break;

    /**
     * @author griffinj@lafayette.edu
     * Islandora search functionality needed to be overridden
     * @see islandora_dss_solr_theme()
     *
     */
    /*
  case 'islandora_solr_advanced_search_form':

    // Push/append a theming callback on to the array of theming callbacks for the individual form element
    $form['#theme'] = array('bootstrap_dss_digital_islandora_solr_advanced_search_form');
    break;
    */

  case 'islandora_solr_simple_search_form':

    $form['simple']['#attributes']['id'] = 'islandora-simple-search-wrapper';

    // Increase the size of the text box
    $form['simple']['islandora_simple_search_query']['#size'] = 21;
    $form['simple']['islandora_simple_search_query']['#value'] = 'Search...';
    $form['simple']['islandora_simple_search_query']['#default_value'] = 'Search...';

    // Insert the glyphicon for searching
    //$form['simple']['submit']['#value'] = '<span class="icon-large"></span>';

    // Default to site-scoped search
    $form['simple']['submit']['#value'] = 'Search this site';

    // Append a button for repository-scoped search
    $form['simple']['submit-1'] = array(
					'#value' => 'Search the collections',
					);

    // Providing a div wrapper for styling
    $form['simple']['submit']['#prefix'] = '<div class="islandora-simple-search-button-container">';
    $form['simple']['submit']['#suffix'] = '</div>';

    $form['simple']['submit-1']['#prefix'] = '<div class="islandora-simple-search-button-1-container">';
    $form['simple']['submit-1']['#suffix'] = '</div>';

    //$form['simple']['submit']['#class'][] = 'icon-search';

    /*
    $form['simple']['#suffix'] = l('Advanced Search', '/', array('attributes' => array('data-toggle' => 'modal'),
								 'class' => array('btn', 'btn-primary', 'btn-lg'),
								 'fragment' => 'advanced-search-modal'));
    */
    //$form['simple']['#suffix'] = '<a href="#advanced-search-modal" data-toggle="lafayette-dss-modal">Advanced Search</a>';



    break;
  }
}

/**
 * Preprocess variables for
 *
 * @see islandora-solr-advanced-search-form.tpl.php
 */
function bootstrap_dss_digital_preprocess_bootstrap_lafayette_lib_dss_islandora_solr_advanced_search_form(&$variables) {

  //$form = $variables['form'];

  //foreach($form['terms'] as $key => &$value) {
  foreach($variables['form']['terms'] as $key => &$value) {

    if(is_numeric($key)) {

      $value['add']['#value'] = '<span class="icon-large"></span>';
      /*
	$value['add']['#value'] = '<span class="icon-large"></span>';
	$value['add']['#ajax']['callback'] = '_bootstrap_dss_digital_islandora_solr_advanced_search_terms';

	$value['search']['#value'] = '';
	//dpm($value);
	*/
    }
  }
}