/**
 * @file A widget for sorting Islandora Objects within an ordered list (<ol>)
 * @author griffinj@lafayette.edu
 *
 */

"use strict";

    /*
    if(ASC) oldASC = ASC;
    if(DESC) oldDESC = DESC;
    */

    // Globals
    /*
    var ASC = true;
    var DESC = false;
    */

    /**
     * Constructor
     *
     */
var LafayetteDssObjectList = function($, element, options) {
    
    this.$ = $;
    this.element = element;
    this.options = $.extend({
	    fieldSelector: '.islandora-inline-metadata dd.solr-value.dc-title',
	    order: 'ASC'
	}, options);
    
    this.fieldSelector = this.options.fieldSelector;
    this.order = this.options.order;

    this.$element = $(element);
    this._index = [];
};

/**
 * ObjectList Object
 *
 */
LafayetteDssObjectList.prototype = {

    constructor: LafayetteDssObjectList,

    sort: function(fieldSelector) {

	fieldSelector = fieldSelector || this.fieldSelector;
	$ = this.$;
	var that = this;

	this._index = this.$element.children('.islandora-solr-search-result').sort(function(u, v) {
		
		return $(u).find(fieldSelector).text().localeCompare($(v).find(fieldSelector).text());
	    });

	if(this.order == 'DESC') {
	    
	    this._index = this._index.reverse();
	}

	this.$element.empty().append(this._index);
    }
};

/**
 * Drupal integration
 *
 */
(function($, Drupal, LafayetteDssObjectList) {

    Drupal.theme.prototype.bootstrapDssObjectList = function() {

	var objectList = new LafayetteDssObjectList($, $('.islandora-solr-search-result-list'));

	$('.islandora-discovery-control.title-sort-control select').change(function() {

		objectList.sort($(this).val());
	    });
    };

    // @todo Refactor
    $(document).ready(function() {
	    
	    Drupal.theme('bootstrapDssObjectList');
	});

})(jQuery, Drupal, LafayetteDssObjectList);
