<?php

  /**
   * @file Object-oriented XML to Array transform
   * @author griffinj@lafayette.edu
   
   * Project-specific
   *
   */

class DssDc extends SimpleXMLElement {

  private function set_namespaces() {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3
  }

  /**
   * Retrieve the collection label from within the administrative MODS element
   *
   */
  private function get_collection_label() {

    $this->set_namespaces();
    $value = '';

    $relation_is_part_of = $this->xpath("./mods:note[@type='admin']");
    
    if(!empty($relation_is_part_of)) {
      
      $element = array_shift($relation_is_part_of);

      $value = (string) $element;

      // @todo Resolve
      // This to extract the actual string value from the Object by casting
      //$this->collection_label = (string) $value;
    }
    
    return $value;
  }

  /**
   * Generate a brief description
   *
   */
  public function description() {

    $values = array();

    $xpaths = array(
		    "./dc:title",
		    "./dc:date[1]",
		    "./dc:coverage",
		    "./dc:description",
		    "./dc:subject",
		    "./dc:rights",
		    "./dc:publisher",
		    "./dc:source",
		    "./dc:type"
		    );

    $collection_label = $this->get_collection_label();

    $this->set_namespaces();
    $values = array();
      
    foreach($xpaths as $xpath) {

      $elements = $this->xpath($xpath);
      
      if(!empty($elements)) {
	
	$element = array_shift($elements);
	$values[] = (string) $element;
      }
    }

    $description = implode(' ', $values);

    return $description;
  }

  public function toArray($label_map=array()) {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    //$this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3
    $this->registerXPathNamespace("dc", "http://purl.org/dc/elements/1.1/");

    $mods_array = array();

    //$relation_is_part_of_value = (string) array_shift($this->xpath("./mods:note[@type='admin']"));

    $template_map = array(
			  'Title' => array('xpath' => "./dc:title",
					   'property' => 'name',
					   'itemprop' => 'name'),

			  'Publication Date' => array('xpath' => "./dc:date[1]",
						      'facet' => true,
						      'field' => 'dc.date',
						      'date' => true,
						      'property' => 'datePublished',
						      'itemprop' => 'datePublished'),

			  'Coverage' => array('xpath' => "./dc:coverage",
					      'property' => 'contentLocation',
					      'itemprop' => 'contentLocation'),

			  'Description' => array('xpath' => "./dc:description",
						 'property' => 'description',
						 'itemprop' => 'description'),

			  'Subject' => array('xpath' => "./dc:subject",
					     'facet' => true,
					     'field' => 'dc.subject',
					     'property' => 'isPartOf',
					     'itemprop' => 'isPartOf'),

			  'Rights' => array('xpath' => "./dc:rights",
					    'property' => 'license',
					    'itemprop' => 'license'),

			  'Publisher' => array('xpath' => "./dc:publisher",
					       'property' => 'publisher',
					       'itemprop' => 'publisher'),

			  'Source Material' => array('xpath' => "./dc:source",
						     'property' => 'associatedMedia',
						     'itemprop' => 'associatedMedia'),

			  'Medium' => array('xpath' => "./dc:type",
					    'facet' => true,
					    'field' => 'dc.type',
					    'property' => 'bookFormat',
					    'itemprop' => 'bookFormat'),
			  );

    //$this->xpath("./mods:note[@type='admin']");

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];

	$is_facet = array_key_exists('facet', $value) ? $value['facet'] : false;

	/**
	 * Hard-coded value
	 * @todo Abstract and refactor
	 *
	 */
	if($is_facet && array_key_exists('field', $value)) {

	  //$facet_field = $value['field'];
	  $facet_field = $value['field'];
	}

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
	}

	if(array_key_exists('link', $value)) {

	  $is_link = $value['link'];
	}

	// Implementing support for RDFa
	if(array_key_exists('property', $value)) {

	  $property = $value['property'];
	}

	// Implementing support for schema.org
	if(array_key_exists('itemprop', $value)) {

	  $itemprop = $value['itemprop'];
	}
      } else {

	$xpath = $value;
      }

      // Transforming the MODS Document into a PHP Array
      //
      $map = function($element) {

        $lang = 'en-US';

        // Need to scrape
        // Refactor
        $m = preg_match('/xml\:lang\="(.+)"/', $element->asXML(), $matches);

        if($m) {

          $lang = $matches[1];
        }

        return array('label' => $element->getName(),
                     'language' => $lang,
                     'value' => (string) $element,
		     //'class' => drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()))
		     'class' => strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()))
		     );
      };

      $mods_elements = array_map($map, $this->xpath($xpath));
      
      for($i = 0; $i < sizeof($mods_elements); $i++) {

	$element = &$mods_elements[$i];

	if($i > 0) {

	  $prev_element = $mods_elements[$i - 1];
	  if($element['label'] == $prev_element['label'] &&
	     $element['language'] == $prev_element['language']) {
	    
	    $element['class'] .= ' metadata-value-set';
	  }
	}
      }

      $collection_facets = 0;

      // Refactor
      foreach($mods_elements as $i => &$element) {

	if($is_facet) {

	  /**
	   * Hard-coded mapping
	   * @todo Refactor
	   *
	   */
	  if(isset($facet_field) && $facet_field) {

	    //$element['field'] = $facet;
	    $element['facet'] = $facet_field;
	  } elseif(array_key_exists($facet, $label_map)) {

	    $element['facet'] = $label_map[$facet];
	  } else {

	    $element['facet'] = $facet;
	  }

	  if($is_date) {

	    $element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  } else {
	  
	    $element['facet_value'] = '"' . $element['value'] . '"';
	  }

	  $facet_params_values = array();
	  $facet_params = array();

	  if($element['facet'] == 'cdm.Relation.IsPartOf') {

	    $collection_elements = $this->xpath("./mods:note[@type='admin']");

	    for($i=0; $i<$collection_facets; $i++) {

	      if($element['value'] != (string) $collection_elements[$i]) {

		$facet_params["f[{$i}]"] = 'cdm.Relation.IsPartOf' . ':"' . (string) $collection_elements[$i] . '"';
	      }
	    }

	    $facet_params["f[{$i}]"] = $element['facet'] . ':"' . $element['value'] . '"';
	    $collection_facets++;
	  } else {

	    /**
	     * @todo Refactor
	     * Resolves DSS-261
	     *
	     */
	    $facet_index = 1;
	    $facet_params["f[0]"] = 'cdm.Relation.IsPartOf:"Lafayette Newspaper Collection"';

	    $facet_params["f[{$facet_index}]"] = $element['facet'] . ':"' . $element['value'] . '"';
	  }

	  $element['facet_href'] = l($element['value'], "islandora/search/*:*", array('query' => $facet_params,
										      ));
	}

	if($is_date) {

	  $element['date_value'] = gmdate('Y-m-d', strtotime($element['value']));

	  $facet_params["f[{$facet_index}]"] = $element['facet'] . ':[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $element['facet_href'] = l($element['date_value'], "islandora/search/*:*", array('query' => $facet_params,
											   ));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}

	if(isset($property)) {

	  $element['property'] = $property;
	}

	if(isset($itemprop)) {

	  $element['itemprop'] = $itemprop;
	}

	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    $collection_elements = array();

    return $mods_array;
  }
  }
