<?php

  /**
   * @file Object-oriented XML to Array transform
   * @author griffinj@lafayette.edu
   
   * Project-specific
   *
   */

class DssMods extends SimpleXMLElement {

  public $collection_label;

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

    $collection_xpath = array(
			      'Lafayette Magazine Collection' => array('./mods:titleInfo/mods:title',
								       './mods:titleInfo/mods:subTitle',
								       './mods:titleInfo/mods:partNumber',
								       './mods:name/mods:displayForm',
								       '(//mods:date[@encoding="w3cdtf"] | //mods:dateOrigin[@encoding="w3cdtf"])[1]',
								       './mods:originInfo/mods:publisher',
								       './mods:originInfo/mods:place/mods:placeTerm',
								       './mods:relatedItem[@type="host"]/mods:titleInfo/mods:title'
								       )
			      );

    $collection_label = $this->get_collection_label();

    if(array_key_exists($collection_label, $collection_xpath)) {

      $this->set_namespaces();
      $xpaths = $collection_xpath[$collection_label];
      $values = array();

      foreach($xpaths as $xpath) {

	$elements = $this->xpath($xpath);

	if(!empty($elements)) {

	  $element = array_shift($elements);
	  $values[] = (string) $element;
	}
      }
    }

    $description = implode(' ', $values);

    return $description;
  }

  public function toArray($label_map = array(), $collection_pid = NULL, $collection_label = NULL) {

    $collection_pid = is_null($collection_pid) ? 'islandora:root' : $collection_pid;
    $collection_label = is_null($collection_label) ? 'Digital Collection at Lafayette' : $collection_label;

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
											     "picture postcard" => 'Picture postcard'),
									     ),
				       'Marquis de Lafayette Prints Collection' => array('form' => array("photoprint" => 'lithograph')));

    // Attempt to identify the collection using the mods:note[@type="admin"] element
    $relation_is_part_of = $this->xpath("./mods:note[@type='admin']");
    if(!empty($relation_is_part_of)) {

      //$relation_is_part_of_value = (string) array_shift($relation_is_part_of);
      $collection_label = (string) array_shift($relation_is_part_of);
    }


    switch($collection_label) {

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

			    //'Description.Caption.English' => "./mods:note[@xml:lang='en-US']",
			    //'Description.Caption.Japanese' => "./mods:note[@xml:lang='en-US']",

			    'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
							   'facet' => true),
			    //'Description.Indicia.English' => "./mods:note[@xml:lang='en-US']",
			    //'Description.Indicia.Chinese' => "./mods:note[@xml:lang='en-US']",
			    //'Description.Indicia.Japanese' => "./mods:note[@xml:lang='en-US']",
			    //'Description.Indicia.Korean' => "./mods:note[@xml:lang='en-US']",

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

			    /*
			    'Creator.Digital' => "./mods:note[@type='creation credits']",
			    'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",
			    //'Rights.Digital' => "./mods:accessCondition",
			    'Format.Digital' => "./mods:note[@type='digital format']",
			    */


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

			    /*
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
			    */
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

			    //'Medium' => "./mods:note[2]",
			    //'Collection' => array('xpath' => "./mods:note[@type='admin']",
			    //'facet' => true),

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

    case 'Lafayette Magazine Collection':

      $template_map = array('Title' => array('xpath' => "./mods:titleInfo/mods:title",
					     'property' => 'name',
					     'itemprop' => 'name'),
			    'Subtitle' => array('xpath' => './mods:titleInfo/mods:subTitle',
						'property' => 'alternativeHeadline',
						'itemprop' => 'alternativeHeadline'),
			    'Part Of' => array('xpath' => './mods:titleInfo/mods:partNumber',
					       'property' => 'alternativeHeadline',
					       'itemprop' => 'alternativeHeadline'),
			    'Author' => array('xpath' => './mods:name/mods:displayForm',
					      'property' => 'author',
					      'itemprop' => 'author'),
			    'Date' => array('xpath' => '(//mods:date[@encoding="w3cdtf"] | //mods:dateOrigin[@encoding="w3cdtf"] )[1]',
					    'facet' => TRUE,
					    'date' => TRUE,
					    'field' => 'MODS.mods.relatedItem.date.w3cdtf_dts',
					    'property' => 'datePublished',
					    'itemprop' => 'datePublished'),
			    'Publisher' => array('xpath' => './mods:originInfo/mods:publisher',
						 'facet' => TRUE,
						 'field' => 'MODS.mods.originInfo.publisher_s',
						 'property' => 'publisher',
						 'itemprop' => 'publisher'),
			    'Place of Publication' => array('xpath' => './mods:originInfo/mods:place/mods:placeTerm',
							    'property' => 'contentLocation',
							    'itemprop' => 'contentLocation'
							    ),
			    'Series' => array('xpath' => './mods:relatedItem[@type="host"]/mods:titleInfo/mods:title',
					      'facet' => TRUE,
					      'field' => 'MODS.mods.relatedItem.titleInfo.title_ss',
					      'property' => 'isPartOf',
					      'itemprop' => 'isPartOf'
					      ),
			    'Note' => array('xpath' => './mods:note[@type="date/sequential designation"]',
					    'property' => 'comment',
					    'itemprop' => 'comment'
					    )
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

      $ISLANDORA_DSS_MODS_FIELDS = array('Historical Photograph Collection' => array('geology_slides_esi.subject' => 'dc.subject',
										     'Medium' => 'dc.type'));

      $collection_field = 'fgs.isMemberOf';
      $collection_elements = array($collection_pid);

      // Refactor
      foreach($mods_elements as $i => &$element) {

	/**
	 * Work-around for DSS-167
	 * Work-around for DSS-170
	 *
	 */

	$element['raw_value'] = $element['value'];

	// Extended handling for the Alumni Publications
	if($collection_label == 'Lafayette Magazine Collection') {

	  // Implemented in order to ensure that, when relatedItem entities feature volume and issue data, these are appended to the value specifying the title of the entity
	  if($facet == 'Series') {

	    $volume_elements = $this->xpath('./mods:relatedItem[@type="host"]/mods:titleInfo/mods:title[text()="' . $element['raw_value'] . '"]/../../mods:part/mods:text[@type="volume"]');

	    if(!empty($volume_elements)) {

	      $volume_element = array_shift($volume_elements);
	      $element['value'] .= ' (vol. ' . (string) $volume_element . ')';

	      $issue_elements = $this->xpath('./mods:relatedItem[@type="host"]/mods:part/mods:text[@type="issue"]');
	    
	      if(!empty($issue_elements)) {

		$issue_element = array_shift($issue_elements);
		$element['value'] = rtrim($element['value'], ')');
		$element['value'] .= ', no. ' . (string) $issue_element . ')';
	      }
	    } else {

	      $date_elements = $this->xpath('(//mods:date[@qualifier="approximate"] | //mods:dateOrigin[@qualifier="approximate"] )[1]');

	      if(!empty($date_elements)) {

		$date_element = array_shift($date_elements);
		//$element['value'] = $date_element;
		$element['value'] .= ' (' . (string) $date_element . ')';
	      }
	    }
	  } elseif($facet == 'Title') {

	    $non_sort_elements = $this->xpath('./mods:titleInfo/mods:nonSort');

	    if(!empty($non_sort_elements)) {

	      $non_sort_element = array_shift($non_sort_elements);
	      $element['value'] = (string) $non_sort_element . ' ' . $element['value'];
	    }
	  }
	}

	if(array_key_exists($collection_label, $ISLANDORA_MARC_METADB_MAP)) {

	  if(array_key_exists($element['label'], $ISLANDORA_MARC_METADB_MAP[$collection_label])) {

	    if(array_key_exists($element['value'], $ISLANDORA_MARC_METADB_MAP[$collection_label][$element['label']])) {
	      
	      $element['value'] = $ISLANDORA_MARC_METADB_MAP[$collection_label][$element['label']][$element['value']];
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

	    // Override for the mods:note[@type="admin"] element containing administrative metadata
	    // @todo Refactor and remove
	    $relation_is_part_of_elements = $this->xpath("./mods:note[@type='admin']");
	    if(!empty($relation_is_part_of_elements)) {

	      /**
	       * Disabled in response to issue
	       * Resolves DSS-251
	       *
	       */
	      //$collection_element_values = array_map(function($element) { return (string) $element; }, $collection_elements);

	      $collection_element = array_shift($relation_is_part_of_elements);
	      $collection_element_value = (string) $collection_element;
	      $collection_elements = array($collection_element_value);
	      $collection_field = 'cdm.Relation.IsPartOf';
	    }

	    $facet_index = 0;
	    //foreach($this->xpath("./mods:note[@type='admin']") as $collection_element) {
	    foreach($collection_elements as $collection_element) {

	      //$facet_params_values[] = (string) $facet_params_value;
	      $facet_params["f[{$facet_index}]"] = $collection_field . ':"' . $collection_element . '"';
	      $facet_index++;
	    }

	    if(array_key_exists($collection_label, $ISLANDORA_DSS_MODS_FIELDS) and array_key_exists($element['facet'], $ISLANDORA_DSS_MODS_FIELDS[$collection_label])) {

	      $facet_params["f[{$facet_index}]"] = $ISLANDORA_DSS_MODS_FIELDS[$collection_label][$element['facet']] . ':"' . $element['raw_value'] . '"';
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
	  
	  /**
	   * Anomalous handling for the field MODS.mods.relatedItem.titleInfo.title_ss
	   * Resolves DSS-413
	   *
	   */
	  if($element['facet'] == 'MODS.mods.relatedItem.titleInfo.title_ss') {

	    $element['facet_href'] = l($element['raw_value'], "islandora/search/*:*", array('query' => $facet_params)) . substr($element['value'], strlen($element['raw_value']) );
	  } else {

	    $element['facet_href'] = l($element['value'], "islandora/search/*:*", array('query' => $facet_params));
	  }
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
	  if($collection_label == 'Historical Photograph Collection' or $collection_label == 'McKelvy House Photograph Collection') {

	    if($date_value->format('m-d') == '01-01') {

	      $element['date_value'] = $date_value->format('Y');
	      if(preg_match('/\d0/', $date_value->format('y')) and $collection_label == 'Historical Photograph Collection' ) {

		$element['date_value'] .= "'s";
	      }
	    }
	  } elseif($collection_label == 'Lafayette Magazine Collection' and $facet == 'Date') {

	    $date_elements = $this->xpath('(//mods:date[@qualifier="approximate"] | //mods:dateOrigin[@qualifier="approximate"] )[1]');

	    if(!empty($date_elements)) {

	      $date_element = array_shift($date_elements);
	      $element['date_value'] = (string) $date_element;
	    }
	  }

	  //$element['facet_value'] = '[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $facet_params["f[{$facet_index}]"] = $element['facet'] . ':[' . $element['value'] . ' TO ' . $element['value'] . ']';
	  $element['facet_href'] = l($element['date_value'], "islandora/search/*:*", array('query' => $facet_params));
	}

	if($is_link) {

	  $element['href'] = $element['value'];
	}

	if($property) {

	  $element['property'] = $property;
	}

	if($itemprop) {

	  $element['itemprop'] = $itemprop;
	}

	$element['label'] = $facet;
      }

      $mods_array = array_merge($mods_array, $mods_elements);
    }

    //$collection_elements = array();
    //$this->mods_array = $mods_array;

    return $mods_array;
  }

  }
