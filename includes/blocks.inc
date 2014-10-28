<?php

  /**
   * @file
   * Theming specific to blocks
   *
   */


/**
 * @author griffinj
 * Implements hook_block_view_MODULE_DELTA_alter(&$data, $block).
 *
 */
function bootstrap_dss_digital_block_view_islandora_solr_advanced_alter(&$data, $block) {

  foreach($data['content']['terms'] as $key => &$value) {

    if(is_numeric($key)) {

      $value['add']['#value'] = '<i class="icon-large"></i>';
      $value['add']['#ajax']['callback'] = '_bootstrap_dss_digital_islandora_solr_advanced_search_terms';

      $value['search']['#value'] = '';
    }
  }
}

/**
 *
 * @author griffinj
 * sharethisGet_button_HTML is the function that creates the ShareThis code
 * It returns the appropriate html based on your settings.
 * @see sharethis_get_button_HTML()
 *
 */
function _bootstrap_dss_digital_sharethis_get_button_HTML($data_options, $mPath, $mTitle) {

  // Inject the extra services.
  foreach ($data_options['option_extras'] as $service) {

    $data_options['services'] .= ',"' . $service . '"';
  }

  // The share buttons are simply spans of the form class='st_SERVICE_BUTTONTYPE' -- "st" stands for ShareThis.
  $type = substr($data_options['buttons'], 4);
  $type = $type == "_" ? "" : check_plain($type);
  $service_array = explode(",", $data_options['services']);
  $st_spans = "";
  foreach ($service_array as $service_full) {
    // Strip the quotes from the element in the array (They are there for javascript)
    $service = explode(":", $service_full);

    // Service names are expected to be parsed by Name:machine_name. If only one
    // element in the array is given, it's an invalid service.
    if (count($service) < 2) {
      continue;
    }

    // Find the service code name.
    $serviceCodeName = substr($service[1], 0, -1);

    // Switch the title on a per-service basis if required.
    $title = $mTitle;
    switch ($serviceCodeName) {
      case 'twitter':
        $title = empty($data_options['twitter_suffix']) ? $mTitle : check_plain($mTitle) . ' ' . check_plain($data_options['twitter_suffix']);
        break;
    }

    // Sanitize the service code for display.
    $display = check_plain($serviceCodeName);

    // Put together the span attributes.
    $attributes = array(
      'st_url' => $mPath,
      'st_title' => $title,
      'class' => 'st_' . $display . $type,
    );
    if ($serviceCodeName == 'twitter') {
      if (!empty($data_options['twitter_handle'])) {
        $attributes['st_via'] = $data_options['twitter_handle'];
      }
    }
    // Only show the display text if the type is set.
    if (!empty($type)) {
      $attributes['displaytext'] = check_plain($display);
    }
    // Render the span tag.
    $st_spans .= theme('html_tag', array(
      'element' => array(
        '#tag' => 'span',
        '#attributes' => $attributes,
        '#value' => '', // It's an empty span tag.
      ),
					 ));

  }

  // These are the ShareThis scripts:
  $st_js_options['switchTo5x'] = $data_options['widget'] == 'st_multi' ? TRUE : FALSE;
  if ($data_options['late_load']) {
    $st_js_options['__st_loadLate'] = TRUE;
  }
  $st_js = "<script type='text/javascript'>";
  foreach ($st_js_options as $name => $value) {
    $st_js .= 'var ' . $name . ' = ' . drupal_json_encode($value) . ';';
  }
  $st_js .= '</script>';

  // Check if we're using SSL or not.
  if (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') {
    $st_js .= "<script type='text/javascript' src='https://ws.sharethis.com/button/buttons.js'></script>";
  }
  else {
    $st_js .= "<script type='text/javascript' src='http://w.sharethis.com/button/buttons.js'></script>";
  }

  // This can only be further themed with JavaScript
  drupal_add_js( drupal_get_path('theme', 'bootstrap_dss_digital') . '/js/sharethis_helper.js');

  // Provide the publisher ID.
  $stlight = drupal_json_encode(array(
    'publisher' => $data_options['publisherID'],
  ));
  $st_js .= "<script type='text/javascript'>stLight.options($stlight);</script>";
  return '<div class="sharethis-wrapper">' . $st_spans . $st_js . '</div>';
}

/**
 * HTML contents for sharethis_block_contents
 * @author griffinj
 *
 * @see sharethis_block_contents()
 * @return string
 *
 */
function _bootstrap_dss_digital_sharethis_block_contents() {

  if (variable_get('sharethis_location', 'content') == 'block') {

    // First get all of the options for the sharethis widget from the database:
    $data_options = sharethis_get_options_array();
    $path = isset($_GET['q']) ? $_GET['q'] : '<front>';
    $mPath = url($_GET['q'], array('absolute' => TRUE));
    $mTitle = drupal_get_title();
    return _bootstrap_dss_digital_sharethis_get_button_HTML($data_options, $mPath, $mTitle);
  }
}

/**
 * @author griffinj
 * Implements hook_block_view_MODULE_DELTA_alter(&$data, $block).
 *
 */
function bootstrap_dss_digital_block_view_sharethis_sharethis_block_alter(&$data, $block) {

  $data['content'] = _bootstrap_dss_digital_sharethis_block_contents();
}

function bootstrap_dss_digital_block_view_islandora_solr_simple_alter(&$data, $block) {

  /**
   * Currently, the widget requires "magic constants" of 15px for width and 16px for height
   * @todo Identify the origin of these bugs (likely related to styling) and resolve
   *
   */
  $data['content']['simple']['#suffix'] = l(t('Advanced Search'), '', array('attributes' => array('data-toggle' => 'lafayette-dss-modal',
												  'data-target' => '#advanced-search-modal',
												  'data-width' => '560px',
												  'data-width-offset' => '-15px',
												  'data-height-offset' => '16px'),
									    'fragment' => ' ',
									    'external' => TRUE));
}
