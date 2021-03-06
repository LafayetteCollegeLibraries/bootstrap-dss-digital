<?php

  /**
   * @file Object-oriented XML to Array transform
   * @author griffinj@lafayette.edu
   * @todo Integrate with ctools to provide an interface through which to define classes (or, abstract and implement a set of interfaces)
   *
   * Project-specific
   *
   */

class DssMods extends SimpleXMLElement {

  public function toArray($label_map = array()) {

    $this->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $this->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

    $mods_array = array();

    /**
     * Work-around for resolving issues related to MARC terms
     *
     * This resolves DSS-167
     * This resolves DSS-170
     */
    $ISLANDORA_MARC_METADB_MAP = array('East Asia Image Collection' => array('form' => array("photonegative" => 'Photographic negative',
											     "photoprint" => 'Photographic print',
											     "slide" => 'Photographic slide',
											     "picture postcard" => 'Picture postcard',
											     ),
									     ),
				       'Marquis de Lafayette Prints Collection' => array('form' => array("photoprint" => 'lithograph',
													 )
											 ),
				       );

    $relation_is_part_of_value = (string) array_shift($this->xpath("./mods:note[@type='admin']"));

    $template_map = array('Genre' => "./mods:genre",
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
			  'Source' => array('xpath' => './mods:location/mods:physicalLocation',
					    'facet' => true),
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

			  'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
						       'facet' => true),

                          'Description.Text.English' => "./mods:abstract[@xml:lang='en-US']",
                          'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
                          'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
                          'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",

                          'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
						   'facet' => true),

			  'Description.Indicia.English' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Chinese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Japanese' => "./mods:note[@xml:lang='en-US']",
			  'Description.Indicia.Korean' => "./mods:note[@xml:lang='en-US']",

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

			  'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
						      'facet' => true,
						      'date' => true),
			  'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
						      'facet' => true,
						      'date' => true),

			  'Relation.SeeAlso' => "./mods:relatedItem[@type='references']/mods:note[@type='citation']",

			  'Creator.Digital' => "./mods:note[@type='creation credits']",

			  'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",

			  'Format.Digital' => "./mods:note[@type='digital format']",

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

    switch($relation_is_part_of_value) {

    case 'East Asia Image Collection':

      $template_map = array(
			    /*
			      East Asia Image Collections
			    */
			    'Title.English' => "./mods:titleInfo/mods:title",
			    'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
			    'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
			    'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
			    'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
			    'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
						   'facet' => true),

			    'Description.Critical' => "./mods:note[@type='content']",

			    'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
							   'facet' => true),

			    'Description.Inscription.Japanese' => './mods:note[@type="handwritten" and @xml:lang="Jpan"]',

			    'Description.Text.English' => './mods:abstract[@xml:lang="en-US"]',
			    'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
			    'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
			    'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",

			    'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
							 'facet' => true),
			    'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
								 'facet' => true),
			    'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
							     'facet' => true),
			    'Relation.SeeAlso' => './mods:relatedItem[@displayLabel="See also" and @type="references"]/mods:note[@type="citation"]',
			    'Contributor' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
						   'facet' => true,
						   'field' => 'eastasia.Contributors.Digital'),

			    'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
							 'field' => 'cdm.Relation.IsPartOf', // Work-around; Resolve using Solr interface
							 'facet' => true),

			    'Description.Citation' => "./mods:note[@type='citation']",
			    'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
						     'field' => 'eastasia.Format.Medium', // Work-around; Resolve using Solr interface
						     'facet' => true),

			    'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
						       'field' => 'eastasia.Creator.Company', // Work-around; Resolve using Solr interface
						       'facet' => true),

			    'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
						     'field' => 'eastasia.Creator.Maker', // Work-around; Resolve using Solr interface
						     'facet' => true),
			    'Format.Extent' => "./mods:physicalDescription/mods:extent",

			    'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
							   'facet' => true,
							   'field' => 'eastasia.Date.Artifact.Upper', // Work-around; Resolve using Solr interface
							   'date' => true),
			    'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
							   'facet' => true,
							   'field' => 'eastasia.Date.Artifact.Lower', // Work-around; Resolve using Solr interface
							   'date' => true),
			    'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
							'facet' => true,
							'field' => 'eastasia.Date.Image.Upper', // Work-around; Resolve using Solr interface
							'date' => true),
			    'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
							'facet' => true,
							'field' => 'eastasia.Date.Image.Lower', // Work-around; Resolve using Solr interface
							'date' => true),
			    'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
						     'facet' => true,
						     'date' => true),
			    'Contributor.Donor' => "./mods:note[@type='acquisition']",

			    );

      break;

    case 'Marquis de Lafayette Prints Collection':

      $template_map = array('Title' => "./mods:titleInfo/mods:title",
			    'Description.Note' => "./mods:note[@type='description']",
			    'Creator' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text() = 'cre']/../../mods:namePart",
					       'facet' => true),
			    'Subject.LCSH' => array('xpath' => "./mods:subject[@authority='lcsh']/mods:topic",
						    'facet' => true),
			    'Publisher.Original' => array('xpath' => "./mods:originInfo/mods:publisher",
							  'facet' => true),
			    'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateCreated",
						      'facet' => true,
						      'date' => true),
			    'Format.Medium' => array('xpath' => './mods:physicalDescription/mods:form',
						     'facet' => true),
			    'Format.Extent' => "./mods:physicalDescription/mods:extent",
			    'Description' => "./mods:abstract",
			    'Description.Provenance' => "./mods:note[@type='ownership']",
			    'Description.Series' => array('xpath' => "./mods:note[@type='series']",
							  'facet' => true),
			    'Identifier.ItemNumber' => array('xpath' => "./mods:identifier[@type='item-number']",
							     'facet' => true),
			    'Rights.Digital' => "./mods:accessCondition",
			    'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",
			    'Source' => array('xpath' => './mods:location/mods:physicalLocation',
					      'facet' => true,
					      'field' => 'mdl_prints.source'),
			    'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
							 'facet' => true,
							 'field' => 'cdm.Relation.IsPartOf'),
			    'Format.Digital' => "./mods:note[@type='digital format']",
			    );
      break;

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

    case 'Geology Department Slide Collection':

      $template_map = array(
			    'Title' => "./mods:titleInfo/mods:title",

			    'Subject' => array('xpath' => "./mods:subject/mods:topic",
					       'facet' => true),

			    'Vantage Point' => array('xpath' => "./mods:note[@type='vantagepoint']",
						     'facet' => true),

			    'Publication Date' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
							'facet' => true,
							'field' => 'geology_slides_esi.date.original',
							'date' => true),

			    'Description' => "./mods:abstract",

			    'Coverage' => "./mods:subject/mods:geographic",

			    'See Also' => "/mods:relatedItem[@type='original']/mods:note[@type='citation']",
			    'See Also' => "/mods:relatedItem[@type='host']/mods:note[@type='citation']",

			    'Collection' => array('xpath' => "./mods:note[@type='admin']",
						  'facet' => true,
						  'field' => 'cdm.Relation.IsPartOf'),

			    'Rights' => "./mods:accessCondition",
			    );
      break;

    case 'McKelvy House Photograph Collection':

      $template_map = array(
			    'Title' => "./mods:titleInfo/mods:title",
			    'Subject' => array('xpath' => "./mods:subject/mods:topic",
					       'facet' => true),
			    'Description' => "./mods:abstract",

			    'Note' => './mods:note[@type="description"]',
			    'Medium' => array('xpath' => "./mods:physicalDescription[2]/mods:form",
					      'field' => 'mdl_prints.format.medium',
					      'facet' => true),

			    'Publication Date' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='display']",
							'field' => 'mckelvy.date.original.display',
							'facet' => true,
							'date' => true),

			    'Dimensions' => array('xpath' => "./mods:physicalDescription[1]/mods:form",
						  'field' => 'mdl_prints.format.medium',
						  'facet' => true),

			    'Digital Publisher' => "./mods:note[@type='statement of responsibility']",
			    'Digital Format' => "./mods:note[@type='digital format']",

			    'Rights' => "./mods:accessCondition",
			    'Collection' => array('xpath' => "./mods:note[@type='admin']",
						  'field' => 'cdm.Relation.IsPartOf',  // Work-around: Refactor
						  'facet' => true),

			    'Source' => "./mods:originInfo/mods:place/mods:placeTerm",
			    );
      break;

    case 'Historical Photograph Collection':

      $template_map = array(
			    'Title' => "./mods:titleInfo/mods:title",
			    'Subject' => array('xpath' => "./mods:subject/mods:topic",
					       'facet' => true),
			    'Description' => "./mods:note[1]",
			    'Publication Date' => array('xpath' => './mods:originInfo/mods:dateCreated[@point="start"]',
							'facet' => true,
							'date' => true),
			    //'Medium' => "./mods:genre",
			    'Medium' => array('xpath' => "./mods:genre",
					      'facet' => true),
			    'Collection' => array('xpath' => "./mods:note[@type='admin']",
						  'field' => 'cdm.Relation.IsPartOf', // Work-around: Refactor
						  'facet' => true),
			    'Source' => array('xpath' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title"),
			    'Rights' => "./mods:accessCondition",
			    'Digital Format' => "./mods:physicalDescription/mods:note",
			    );
      break;

    case 'Lafayette World War II Casualties':

      $template_map = array(
			    'Name' => "./mods:titleInfo/mods:title",

			    'Graduating Class' => array('xpath' => './mods:note[@type="lafayette-class"]',
							'facet' => true,
							'field' => 'war_casualties.description.class'), // Work-around: Refactor
			    'Date of Birth' => array('xpath' => './mods:originInfo/mods:dateOther[@type="birth-display"]',
						     'facet' => true,
						     'field' => 'war_casualties.date.birth.display', //  Work-around: Refactor
						     'date' => true),
			    'Place of Birth' => array('xpath' => './mods:note[@type="place-birth"]',
						      'facet' => true,
						      'field' => 'war_casualties.coverage.place.birth'), // Work-around: Refactor
			    'Branch' => array('xpath' => "./mods:note[@type='military-branch']",
					      'facet' => true,
					      'field' => 'war_casualties.description.military.branch'), // Work-around: Refactor
			    'Rank' => array('xpath' => "./mods:note[@type='military-rank']",
					    'facet' => true,
					    'field' => 'war_casualties.description.military.rank'), // Work-around: Refactor
			    'Unit' => array('xpath' => "./mods:note[@type='military-unit']",
					    'facet' => true,
					    'field' => 'war_casualties.contributor.military.unit'), // Work-around: Refactor

			    'Date of Death' => array('xpath' => './mods:originInfo/mods:dateOther[@type="death-display"]',
						     'facet' => true,
						     'field' => 'war_casualties.date.death.display', //  Work-around: Refactor
						     'date' => true),

			    'Place of Death' => array('xpath' => "./mods:note[@type='place-death']",
						      'facet' => true,
						      'field' => 'war_casualties.coverage.place.death'), // Work-around: Refactor

			    'Cause of Death' => "./mods:note[@type='cause-death']",

			    'Honors' => array('xpath' => "./mods:note[@type='military-honors']",
					      'facet' => true,
					      'field' => 'war_casualties.description.honors'),

			    'Collection' => array('xpath' => "./mods:note[@type='admin']",
						  'facet' => true,
						  'field' => 'cdm.Relation.IsPartOf'), // Work-around: Refactor

			    'Source Material' => "./mods:physicalDescription/mods:note[@type='format-analog']",

			    'Digital Format' => "./mods:note[@type='digital format']",
			    'Digital Publisher' => "./mods:note[@type='statement of responsibility']",

			    'Rights' => "./mods:accessCondition",
			    );

      break;

    case 'Lafayette Newspaper Collection':

      $template_map = array('Title' => "./mods:titleInfo/mods:title",
			    'Publication Date' => array('xpath' => "./mods:originInfo/mods:dateIssued",
							'facet' => true,
							'date' => true),
			    'Coverage' => "./mods:subject/mods:hierarchicalGeographic/mods:continent",
			    'Description' => "./mods:note",
			    'Subject' => "./mods:genre",
			    'Rights' => "./mods:accessCondition",
			    'Publisher' => "./mods:originInfo/mods:publisher",
			    'Source Material' => array('xpath' => "./mods:subject/mods:topic",
						       'facet' => true),
			    'Medium' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title",
			    );
      break;

    default:

      // Newspapers and other anomalous Objects
      $template_map = array('Title' => "./mods:titleInfo/mods:title",
			    'Publication Date' => array('xpath' => "./mods:originInfo/mods:dateOther",
							'facet' => true,
							'date' => true),
			    'Coverage' => "./mods:subject/mods:geographic",
			    'Description' => "./mods:note",
			    'Subject' => "./mods:genre",
			    'Rights' => "./mods:accessCondition",
			    'Publisher' => "./mods:originInfo/mods:publisher",
			    'Source Material' => array('xpath' => "./mods:subject/mods:topic",
						       'facet' => true),
			    'Medium' => "./mods:relatedItem[@type='original']/mods:titleInfo/mods:title",
			    );

      break;
    }

    /**
     * Provide handling for datestamps and metadata field values linked to Solr queries
     *
     */

    foreach($template_map as $facet => $value) {

      $is_facet = false;
      $is_date = false;
      $is_link = false;

      if(is_array($value)) {

	$xpath = $value['xpath'];
	$is_facet = array_key_exists('facet', $value) and $value['facet'];

	/**
	 * Hard-coded value
	 * @todo Abstract and refactor
	 *
	 */
	if($is_facet && array_key_exists('field', $value)) {

	  $facet_field = $value['field'];
	}

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
      $map = function($element) {

        $lang = 'en-US';

        // Work-around using scraping
        //! @todo Refactor
        $m = preg_match('/xml\:lang\="(.+)"/', $element->asXML(), $matches);

        if($m) {

          $lang = $matches[1];
        }

        return array('label' => $element->getName(),
                     'language' => $lang,
                     'value' => (string) $element,
		     'class' => strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $element->getName()))
		     );
      };

      $mods_elements = array_map($map, $this->xpath($xpath));

      /**
       * Providing labeling for individual metadata fields
       * (Also accounts for language or character set)
       * @todo This should be abstracted; Perhaps an additional Object should be used for these mapping
       *
       */
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

      /**
       * Provides an overriding map for specific collections
       * @todo Refactor with variable_get().
       *
       */
      //
      $ISLANDORA_DSS_MODS_FIELDS = array('Historical Photograph Collection' => array('geology_slides_esi.subject' => 'dc.subject',
										     'Medium' => 'dc.type'));

      // Refactor
      foreach($mods_elements as $i => &$element) {

	/**
	 * Work-around for DSS-167
	 * Work-around for DSS-170
	 *
	 */

	$element['raw_value'] = $element['value'];
	if(array_key_exists($relation_is_part_of_value, $ISLANDORA_MARC_METADB_MAP)) {

	  if(array_key_exists($element['label'], $ISLANDORA_MARC_METADB_MAP[$relation_is_part_of_value])) {

	    if(array_key_exists($element['value'], $ISLANDORA_MARC_METADB_MAP[$relation_is_part_of_value][$element['label']])) {

	      $element['value'] = $ISLANDORA_MARC_METADB_MAP[$relation_is_part_of_value][$element['label']][$element['value']];
	    }
	  }
	}

	// If this field is a facet...
	if($is_facet) {

	  /**
	   * Hard-coded mapping
	   * @todo Refactor
	   *
	   */

	  // ...and if the Solr field was explicitly given within the array...
	  if(isset($facet_field) && $facet_field) {

	    $element['facet'] = $facet_field;
	  } elseif(array_key_exists($facet, $label_map)) {

	    // ...and if the Solr field was mapped using islandora_solr_fields()
	    $element['facet'] = $label_map[$facet];
	  } else {

	    // ...just set the facet to the human-readable name
	    $element['facet'] = $facet;
	  }

	  // Provide proper facet query structure for date ranges
	  if($is_date) {

	    $element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  } else {

	    $element['facet_value'] = '"' . $element['value'] . '"';
	  }

	  $facet_params_values = array();
	  $facet_params = array();

	  /**
	   * If this is the facet used for storing relationships between collections (i. e. the ContentDM field Relation.IsPartOf), nest the links
	   *
	   */
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

	    $collection_elements = $this->xpath("./mods:note[@type='admin']");

	    /**
	     * Disabled in response to issue
	     * Resolves DSS-251
	     *
	     */
	    $collection_elements = array(array_shift($collection_elements));

	    $facet_index = 0;
	    foreach($collection_elements as $collection_element) {

	      $facet_params["f[{$facet_index}]"] = 'cdm.Relation.IsPartOf' . ':"' . (string) $collection_element . '"';
	      $facet_index++;
	    }

	    if(array_key_exists($relation_is_part_of_value, $ISLANDORA_DSS_MODS_FIELDS) and array_key_exists($element['facet'], $ISLANDORA_DSS_MODS_FIELDS[$relation_is_part_of_value])) {

	      $facet_params["f[{$facet_index}]"] = $ISLANDORA_DSS_MODS_FIELDS[$relation_is_part_of_value][$element['facet']] . ':"' . $element['raw_value'] . '"';
	    } else {

	      /**
	       * Work-around for DSS-167
	       * Work-around for DSS-170
	       *
	       */
	      $facet_params["f[{$facet_index}]"] = $element['facet'] . ':"' . $element['raw_value'] . '"';
	    }
	  }

	  // Construct the URL for the islandora/search query path
	  $element['facet_href'] = l($element['value'], "islandora/search/*:*", array('query' => $facet_params));
	}

	if($is_date) {

	  // Handling for improperly structured datestamps
	  // Resolves DSS-349
	  if(!preg_match('/^\d{4}\-\d{2}\-(.+)/', $element['value'])) {

	    try {

	      // Attempt to transform the date
	      $element['value'] = preg_replace('/^(\d{4})(\d{2})\d{2}\-\d{2}(.+)/', '\1-\2\3', $element['value']);
	      $date_value = new DateTime($element['value'], new DateTimeZone('GMT'));
	    } catch(Exception $e) {

	      $date_value = new DateTime('1970-01-01', new DateTimeZone('GMT'));
	    }
	  } else {

	    $date_value = new DateTime($element['value'], new DateTimeZone('GMT'));
	  }
	  //$element['date_value'] = date_format($date_value, 'Y-m-d');
	  $element['date_value'] = $date_value->format('Y-m-d');

	  /**
	   * Addressing the issue for ...
	   * Resolves DSSSM-885
	   */
	  if($relation_is_part_of_value == 'Historical Photograph Collection' or $relation_is_part_of_value == 'McKelvy House Photograph Collection') {

	    if($date_value->format('m-d') == '01-01') {

	      $element['date_value'] = $date_value->format('Y');
	      if(preg_match('/\d0/', $date_value->format('y')) and $relation_is_part_of_value == 'Historical Photograph Collection' ) {

		$element['date_value'] .= "'s";
	      }
	    }
	  }

	  //$element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $facet_params["f[{$facet_index}]"] = $element['facet'] . ':[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $element['facet_href'] = l($element['date_value'], "islandora/search/*:*", array('query' => $facet_params));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}

	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    $collection_elements = array();
    return $mods_array;
  }
}
