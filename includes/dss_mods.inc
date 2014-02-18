<?php

  /**
   * @file Object-oriented XML to Array transform
   * @author griffinj@lafayette.edu
   
   * Project-specific
   *
   */

class DssMods extends SimpleXMLElement {

  public function toArray($label_map=array()) {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

    $mods_array = array();

    /*
    $template_map = array(
			  // Generic
			  'Title' => "./mods:titleInfo/mods:title",
                          'Genre' => "./mods:genre",
                          'Location' => "./mods:subject/mods:geographic",
                          'Topic' => "./mods:subject/mods:topic",
                          'Note' => "./mods:note",
                          'Publisher' => "./mods:originInfo/mods:publisher",
                          'Date of Publication' => "(./mods:originInfo/mods:dateOther)[1]",
                          'Date of Digitization' => "(./mods:originInfo/mods:dateOther)[2]",
                          'Rights' => "./mods:accessCondition",
			  );
    */

    /*


    dc.title (String, 13 characters ) Title.English
    eastasia.Subject.OCM (String, 11 characters ) Subject.OCM
    eastasia.Contributors.Digital (String, 20 characters ) Contributors.Digital
    eastasia.Coverage.Location (String, 17 characters ) Coverage.Location
    eastasia.Coverage.Location.Country (String, 25 characters ) Coverage.Location.Country
    eastasia.Format.Medium (String, 13 characters ) Format.Medium
    eastasia.Date.Artifact.Lower (String, 19 characters ) Date.Artifact.Lower
    eastasia.Date.Artifact.Upper (String, 19 characters ) Date.Artifact.Upper
    eastasia.Date.Image.Lower (String, 16 characters ) Date.Image.Lower
    eastasia.Date.Image.Upper (String, 16 characters ) Date.Image.Upper
    dc.contributor (String, 14 characters ) dc.contributor
    dc.coverage (String, 11 characters ) dc.coverage
    dc.date (String, 7 characters ) dc.date
    dc.description (String, 14 characters ) dc.description
    dc.format (String, 9 characters ) dc.format
    dc.identifier (String, 13 characters ) dc.identifier
    dc.language (String, 11 characters ) dc.language
    dc.publisher (String, 12 characters ) dc.publisher
    dc.rights (String, 9 characters ) dc.rights
    dc.source (String, 9 characters ) dc.source
    dc.subject (String, 10 characters ) dc.subject
    dc.type (String, 7 characters ) dc.type
    mdl_prints.subject.lcsh (String, 12 characters ) Subject.LCSH
    mdl_prints.format.medium (String, 13 characters ) Format.Medium
    mdl_prints.format.extent (String, 13 characters ) Format.Extent
    mdl_prints.description (String, 11 characters ) Description
    mdl_prints.description.provenance (String, 22 characters ) Description.Provenance
    mdl_prints.description.series (String, 18 characters ) Description.Series
    mdl_prints.identifier.itemnumber (String, 21 characters ) Identifier.ItemNumber
    geology_slides.coverage.location.country (String, 25 characters ) coverage.location.country
    geology_slides.coverage.location.state (String, 23 characters ) coverage.location.state
    geology_slides.description.geologic.feature (String, 28 characters ) description.geologic.feature
    geology_slides.description.critical (String, 20 characters ) description.critical
    geology_slides_esi.subject (String, 7 characters ) Subject
    geology_slides_esi.description.vantagepoint (String, 24 characters ) Description.Vantagepoint
    geology_slides_esi.date.original (String, 13 characters ) Date.Original
    mdl_prints.rights.digital (String, 14 characters ) Rights.Digital
    mdl_prints.publisher.digital (String, 17 characters ) Publisher.Digital
    mdl_prints.format.digital (String, 14 characters ) Format.Digital
    cdm.Relation.IsPartOf (String, 17 characters ) Relation.IsPartOf


     */

    $relation_is_part_of_value = (string) array_shift($this->xpath("./mods:note[@type='admin']"));
    dpm( $relation_is_part_of_value  );

    switch($relation_is_part_of_value) {

      case 'Presidents of Lafayette College':

      $template_map = array(
			  /*
			   * Presidents
			   *
			   */
			  'Title' => "./mods:titleInfo/mods:title",
                          'Subject' => array('xpath' => "./mods:subject/mods:topic",
						 'facet' => true),
                          'Description' => array('xpath' => "./mods:note",
						 'facet' => true),
			  'Publisher' => "./mods:originInfo/mods:publisher",
			  'Digital Publisher' => array('xpath' => "./mods:name/mods:namePart",
						       'facet' => true),
			  'Digitization Date' => array('xpath' => "./mods:originInfo/mods:dateOther",
						       'facet' => true,
						       'date' => true),
			  'Digital Format' => array('xpath' => "./mods:physicalDescription/mods:note",
						    'facet' => true),
			  'Source' => array('xpath' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title"),
			  'Language' => array('xpath' => "./mods:language/mods:languageTerm[@type='text']"),
                          'Rights' => "./mods:accessCondition",
                          );
      break;

    case 'Historical Photograph Collection':
      
      $template_map = array(
			    'Title' => "./mods:titleInfo/mods:title",
			    'Subject' => array('xpath' => "./mods:subject/mods:topic",
					       'facet' => true),
			    'Description' => "./mods:note[1]",
			    'Publication Date' => array('xpath' => "./mods:originInfo/mods:dateOther",
							'facet' => true,
							'date' => true),
			    'Medium' => "./mods:note[2]",
			    'Collection' => array('xpath' => "./mods:note[@type='admin']",
						  'facet' => true),
			    'Source' => array('xpath' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title"),
			    'Rights' => "./mods:accessCondition",
			    'Digital Format' => array('xpath' => "./mods:physicalDescription/mods:note",
						      'facet' => true),
			    );
      break;
    default:

      $template_map = array(
			    'Genre' => "./mods:genre",
			    'Location' => "./mods:subject/mods:geographic",
			    'Topic' => "./mods:subject/mods:topic",
			    //'Note' => "./mods:note",
			    'Publisher' => "./mods:originInfo/mods:publisher",
			    'Date of Publication' => "(./mods:originInfo/mods:dateOther)[1]",
			    'Date of Digitization' => "(./mods:originInfo/mods:dateOther)[2]",
			    'Rights' => "./mods:accessCondition",

			  /*
			    East Asia Image Collections
			   */
			  'Title' => "./mods:titleInfo/mods:title",
			  'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
                          'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
                          'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
                          'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
                          'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
						 'facet' => true),

			  /*
			    MDL
			  */
			  'Source' => './mods:location/mods:physicalLocation',
			  'Subject.LCSH' => array('xpath' => "./mods:subject[@authority='lcsh']/mods:topic",
						  'facet' => true),

			  'Format.Extent' => "./mods:physicalDescription/mods:extent",
                          'Description' => "./mods:abstract",
			  'Description.Series' => array('xpath' => "./mods:note[@type='series']",
							'facet' => true),

			  'Identifier.ItemNumber' => "./mods:identifier[@type='item-number']",

                          'Description.Citation' => "./mods:note[@type='citation']",
                          'Description.Critical' => "./mods:note[@type='content']",
			  'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
							 'facet' => true),
			  'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
							       'facet' => true),
                          'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
							   'facet' => true),
                          'Contributors.Digital' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
							  'facet' => true),
			  //			  'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
			  //			       'facet' => false,
			  //			       'link' => true,
			  //			       'href' => ''),

			  'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
						       'facet' => true),

                          'Description.Text.English' => "./mods:abstract[@xml:lang='en-US']",
                          'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
                          'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
                          'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",



			  /*
                          'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
						     'facet' => true),
			  */

			  // #{maker}</namePart><role><roleTerm authority='marcrelator' type='code'>pht</roleTerm></role></name>"
                          //'Creator.Maker' => "./mods:name/mods:namePart",
                          'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
						   'facet' => true),

			  'Description.Indicia.English' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Chinese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Japanese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Korean' => "./mods:note[@xml:lang='en-US']",

			  /*
			  'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
						       'facet' => true),
			  */

			  'Format.Extent' => "./mods:physicalDescription/mods:extent",
			  'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
						   'facet' => true),
			  'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
						   'facet' => true,
						   'date' => true),
			  'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
							 'facet' => true,
							 'date' => true),
			  'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
							 'facet' => true,
							 'date' => true),

			  //'Date.Search' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='search']",
			  //'facet' => true,
			  //'date' => true),

			  'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
						      'facet' => true,
						      'date' => true),
			  'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
						      'facet' => true,
						      'date' => true),

			  'Relation.SeeAlso' => "./mods:relatedItem[@type='references']/mods:note[@type='citation']",

			  'Creator.Digital' => "./mods:note[@type='creation credits']",

			  'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",

			  //'Rights.Digital' => "./mods:accessCondition",

			  'Format.Digital' => "./mods:note[@type='digital format']",

			  /*
			  'Identifier.Download' => array('xpath' => "./mods:location/mods:url[@displayLabel='Download']",
							 'facet' => false,
							 'link' => true,
							 'href' => '%value'),
			  */

			  // DSS-116
			  //'Identifier.Zoom' => './mods:location/mods:url[@displayLabel="Zoom"]',

			  /*
			    war-casualties
			   */

			  'Description.class' => array('xpath' => './mods:note[@type="lafayette-class"]',
						       'facet' => true),

			  'Date.birth.display' => array('xpath' => './mods:originInfo/mods:dateOther[@type="birth-display"]',
							'facet' => true,
							'date' => true),

			  'Coverage.place.birth' => array('xpath' => './mods:note[@type="place-birth"]',
							  'facet' => true),



			  'Description.military.branch' => array('xpath' => "./mods:note[@type='military-branch']",
								  'facet' => true),

			  'Description.military.rank' => array('xpath' => "./mods:note[@type='military-rank']",
								'facet' => true),

			  'Description.military.unit' => array('xpath' => "./mods:note[@type='military-unit']",
								'facet' => true),

			  'Date.death.display' => array('xpath' => './mods:originInfo/mods:dateOther[@type="death-display"]',
							'facet' => true,
							'date' => true),

			  'Coverage.place.death' => array('xpath' => "./mods:note[@type='place-death']",
							  'facet' => true),

			  'Description.cause.death' => "./mods:note[@type='cause-death']",

			  'Description.honors' => array('xpath' => "./mods:note[@type='military-honors']",
							'facet' => true),

			  'Format.analog' => "./mods:note[@type='format-analog']",
                          );
      break;
    }

    //$this->xpath("./mods:note[@type='admin']");

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = $value['facet'];

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
	}

	if(array_key_exists('link', $value)) {

	  $is_link = $value['link'];
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

      // Refactor
      foreach($mods_elements as $i => &$element) {

	if($is_facet) {

	  /*
	  if(array_key_exists($facet, $label_map)) {

	    $element['facet'] = $label_map[$facet];
	  } else {

	    $element['facet'] = $facet;
	  }
	  */
	  $element['facet'] = $facet;

	  if($is_date) {

	    $element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  } else {
	  
	    $element['facet_value'] = '"' . $element['value'] . '"';
	  }
	}

	if($is_date) {

	  $element['date_value'] = date('Y-m-d', strtotime($element['value']));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}
	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    $collection_elements = array();

    // Filter MODS elements based upon the parent collection
    if(array_key_exists('', $mods_elements)) {

      /*
      switch($mods_elements['Relation.IsPartOf']) {

      case 'Historical Photograph Collection':

	$collection_elements = array(
				     'Title',
				     'Genre',
				     'Description',
				     'Date of Publication',
				     'Type',
				     'Relation.IsPartOf',
				     'Source',
				     'Rights',
				     'Format.Digital',
				     );

	$_mods_elements = array();
	foreach($mods_elements as $mods_element) {

	  in_array($mods_element['label'], $collection_elements);
	  $_mods_elements[] = $mods_element;
	}

	$mods_elements = $_mods_elements;

	break;

      default:
      
	break;
      }
    }
      */

    }

    return $mods_array;
  }
  }

class McKelvyMods extends DssMods {

}

class GeologySlidesEsiMods extends DssMods {

  
}

class WarCasualtiesMods extends DssMods {

  /*
    Title.nameTitle
    war_casualties.description.classGraduating Class
    Date.birth.displayDate of Birth
    war_casualties.coverage.place.birthPlace of Birth
    war_casualties.description.military.branchBranch
    war_casualties.description.military.rankRank
    war_casualties.contributor.military.unitUnit
    Date.death.displayDate of Death
    war_casualties.coverage.place.deathPlace of Death
    war_casualties.description.cause.deathCause of Death
    war_casualties.description.honorsHonors
    Relation.IsPartOfCollection
    war_casualties.format.analogSource Material
    Format.digitalDigital Format
    Publisher.digitalDigital Publisher
    Rights.digitalRights
   */

}

class PresidentsMods extends DssMods {

  /*
    dc:titleTitle
    dc:subjectSubject
    dc:descriptionDescription
    dc:publisherPublisher
    dc:contributorDigital Publisher
    dc:dateDigitization Date
    dc:formatDigital Format
    dc:sourceSource 
    dc:languageLanguage
    dc:rightsRights
   */

  public function toArray($label_map=array()) {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

    $mods_array = array();

    /*
    $template_map = array(
			  // Generic
			  'Title' => "./mods:titleInfo/mods:title",
                          'Genre' => "./mods:genre",
                          'Location' => "./mods:subject/mods:geographic",
                          'Topic' => "./mods:subject/mods:topic",
                          'Note' => "./mods:note",
                          'Publisher' => "./mods:originInfo/mods:publisher",
                          'Date of Publication' => "(./mods:originInfo/mods:dateOther)[1]",
                          'Date of Digitization' => "(./mods:originInfo/mods:dateOther)[2]",
                          'Rights' => "./mods:accessCondition",
			  );
    */

    /*


    dc.title (String, 13 characters ) Title.English
    eastasia.Subject.OCM (String, 11 characters ) Subject.OCM
    eastasia.Contributors.Digital (String, 20 characters ) Contributors.Digital
    eastasia.Coverage.Location (String, 17 characters ) Coverage.Location
    eastasia.Coverage.Location.Country (String, 25 characters ) Coverage.Location.Country
    eastasia.Format.Medium (String, 13 characters ) Format.Medium
    eastasia.Date.Artifact.Lower (String, 19 characters ) Date.Artifact.Lower
    eastasia.Date.Artifact.Upper (String, 19 characters ) Date.Artifact.Upper
    eastasia.Date.Image.Lower (String, 16 characters ) Date.Image.Lower
    eastasia.Date.Image.Upper (String, 16 characters ) Date.Image.Upper
    dc.contributor (String, 14 characters ) dc.contributor
    dc.coverage (String, 11 characters ) dc.coverage
    dc.date (String, 7 characters ) dc.date
    dc.description (String, 14 characters ) dc.description
    dc.format (String, 9 characters ) dc.format
    dc.identifier (String, 13 characters ) dc.identifier
    dc.language (String, 11 characters ) dc.language
    dc.publisher (String, 12 characters ) dc.publisher
    dc.rights (String, 9 characters ) dc.rights
    dc.source (String, 9 characters ) dc.source
    dc.subject (String, 10 characters ) dc.subject
    dc.type (String, 7 characters ) dc.type
    mdl_prints.subject.lcsh (String, 12 characters ) Subject.LCSH
    mdl_prints.format.medium (String, 13 characters ) Format.Medium
    mdl_prints.format.extent (String, 13 characters ) Format.Extent
    mdl_prints.description (String, 11 characters ) Description
    mdl_prints.description.provenance (String, 22 characters ) Description.Provenance
    mdl_prints.description.series (String, 18 characters ) Description.Series
    mdl_prints.identifier.itemnumber (String, 21 characters ) Identifier.ItemNumber
    geology_slides.coverage.location.country (String, 25 characters ) coverage.location.country
    geology_slides.coverage.location.state (String, 23 characters ) coverage.location.state
    geology_slides.description.geologic.feature (String, 28 characters ) description.geologic.feature
    geology_slides.description.critical (String, 20 characters ) description.critical
    geology_slides_esi.subject (String, 7 characters ) Subject
    geology_slides_esi.description.vantagepoint (String, 24 characters ) Description.Vantagepoint
    geology_slides_esi.date.original (String, 13 characters ) Date.Original
    mdl_prints.rights.digital (String, 14 characters ) Rights.Digital
    mdl_prints.publisher.digital (String, 17 characters ) Publisher.Digital
    mdl_prints.format.digital (String, 14 characters ) Format.Digital
    cdm.Relation.IsPartOf (String, 17 characters ) Relation.IsPartOf
     */

    /**
     *
     */
    if((string) $this->xpath("./mods:note[@type='admin']") == 'Presidents of Lafayette College') {

      $template_map = array(
			  /*
			   * Presidents
			   *
			   */
			  'Title' => "./mods:titleInfo/mods:title",
                          'Subject' => array('xpath' => "./mods:subject/mods:topic",
						 'facet' => true),
                          'Description' => array('xpath' => "./mods:note",
						 'facet' => true),
			  'Publisher' => "./mods:originInfo/mods:publisher",
			  'Digital Publisher' => array('xpath' => "./mods:name/mods:namePart",
						       'facet' => true),
			  'Digitization Date' => array('xpath' => "./mods:originInfo/mods:dateOther",
						       'facet' => true,
						       'date' => true),
			  'Digital Format' => array('xpath' => "./mods:physicalDescription/mods:note",
						    'facet' => true),
			  'Source' => array('xpath' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title"),
			  'Language' => array('xpath' => "./mods:language/mods:languageTerm[@type='text']"),
                          'Rights' => "./mods:accessCondition",
                          );
    }

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = $value['facet'];

	if(array_key_exists('date', $value)) {

	  $is_date = $value['date'];
	}

	if(array_key_exists('link', $value)) {

	  $is_link = $value['link'];
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

      // Refactor
      foreach($mods_elements as $i => &$element) {

	if($is_facet) {

	  if(array_key_exists($facet, $label_map)) {

	    $element['facet'] = $label_map[$facet];
	  } else {

	    $element['facet'] = $facet;
	  }

	  if($is_date) {

	    $element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  } else {
	  
	    $element['facet_value'] = '"' . $element['value'] . '"';
	  }
	}

	if($is_date) {

	  $element['date_value'] = date('Y-m-d', strtotime($element['value']));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}
	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    $collection_elements = array();

    // Filter MODS elements based upon the parent collection
    if(array_key_exists('', $mods_elements)) {

      switch($mods_elements['Relation.IsPartOf']) {

      case 'Historical Photograph Collection':

	$collection_elements = array(
				     'Title',
				     'Genre',
				     'Description',
				     'Date of Publication',
				     'Type',
				     'Relation.IsPartOf',
				     'Source',
				     'Rights',
				     'Format.Digital',
				     );

	$_mods_elements = array();
	foreach($mods_elements as $mods_element) {

	  in_array($mods_element['label'], $collection_elements);
	  $_mods_elements[] = $mods_element;
	}

	$mods_elements = $_mods_elements;

	break;

      default:
      
	break;
      }
    }


    return $mods_array;
  }
}
