/**
 * jQuery functionality for the theme
 * @author griffinj@lafayette.edu
 *
 */

(function($, Drupal) {

    'use strict';

    // Following the Drupal.theme implementation
    // Please see https://drupal.org/node/304258
    Drupal.theme.prototype.bootstrapDssLdr = function() {

	/**
	 * @author griffinj
	 * Ensure that the navbar collapse is triggered
	 *
	 */
	$('#menu-toggle-icon').click(function(e) {

		//$('.nav-collapse').collapse();
		$('.nav-collapse').collapse('toggle');
	    });

	/**
	 * Global handler for smartphone devices
	 * Refactor?
	 *
	 */
	var smartPhoneHandler = function($) {

	    /**
	     * This ensures that the responsive navbar is set at a fixed pixel width when resized below 480
	     * @see DSSSM-313
	     *
	     */
	    if( !/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion) ) {

		//if($( window ).width() <= 510) {
		//if($( window ).width() <= 494) {
		//if($( window ).width() <= 459) {
		//if($( window ).width() <= 478) {
		//if($( window ).width() <= 508) {
		if($( window ).width() <= 534) {

		    $('.navbar-inner').addClass('navbar-fixed-width');		    
		} else {

		    $('.navbar-inner').removeClass('navbar-fixed-width');
		}
	    }

	    // Ensure that the menu items are displayed in a format appropriate to smartphone and tablet devices
	    if($( window ).width() <= 480) {

		$('.navbar-inner-container').insertAfter($('.menu-toggle-container'));

		//} else if($( window ).width() < 1008) {
	    } else if($( window ).width() < 993) {

		
		//$('.navbar-inner-container').removeClass('desktop');
		$('.navbar-inner-container').insertAfter($('.menu-toggle-container'));
		//$('.navbar-inner-container').addClass('tablet');
	    } else {

		$('.navbar-inner-container').insertBefore($('.auth-share-simple-search-container'));
		//$('.navbar-inner-container').addClass('desktop');
		//$('#menu-toggle-control-container').css('display', 'none');
	    }


	    // Adjust the DSS link in response to the size of the browser
	    //if($( window ).width() <= 754 ) {
	    if($( window ).width() <= 736 ) {

		// Refactor
		/*
		if($('#navbar .navbar-header h1 a').text() != 'DSS') {

		    $(document).data('Drupal.theme.bootstrap.dss', $('#navbar .navbar-header h1 a').text());
		    $('#navbar .navbar-header h1 a').text('DSS');
		}
		*/

		$('#navbar .navbar-header h1 a').addClass('navbar-header-collapsed');
	    } else {

		/*
		if($('#navbar .navbar-header h1 a').text() == 'DSS') {

		    $('#navbar .navbar-header h1 a').text( $(document).data('Drupal.theme.bootstrap.dss'));
		}
		*/

		$('#navbar .navbar-header h1 a').removeClass('navbar-header-collapsed');
	    }

	    /**
	     * @todo Refactor all
	     *
	     */
	    //if($( window ).width() <= 480 ) {
	    if($( window ).width() <= 470 ) {

		$('#carousel-featured-collection .carousel-caption-heading').addClass('carousel-collapsed');
		$('#carousel-featured-collection .carousel-caption-text').addClass('carousel-collapsed');
	    } else {

		$('#carousel-featured-collection .carousel-caption-heading').removeClass('carousel-collapsed');
		$('#carousel-featured-collection .carousel-caption-text').removeClass('carousel-collapsed');
	    }

	    if($( window ).width() <= 484 ) {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').addClass('navbar-header-top-collapsed');
		$('#navbar .navbar-header h1 a').addClass('navbar-header-collapsed-1');
	    } else {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').removeClass('navbar-header-top-collapsed');
		$('#navbar .navbar-header h1 a').removeClass('navbar-header-collapsed-1');
	    }

	    if($( window ).width() <= 364 ) {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').addClass('navbar-header-top-collapsed-1');
		$('#navbar .navbar-header h1 a').addClass('navbar-header-collapsed-2');
	    } else {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').removeClass('navbar-header-top-collapsed-1');
		$('#navbar .navbar-header h1 a').removeClass('navbar-header-collapsed-2');
	    }

	    if($( window ).width() <= 300 ) {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').addClass('navbar-header-top-collapsed-smallest');
		$('#navbar .navbar-header h1 a').addClass('navbar-header-collapsed-smallest');
	    } else {

		$('html.js body.html header#navbar.navbar div.navbar-header h2').removeClass('navbar-header-top-collapsed-smallest');
		$('#navbar .navbar-header h1 a').removeClass('navbar-header-collapsed-smallest');
	    }

	    //if($( window ).width() >= 1008) {
	    if($( window ).width() >= 993) {

		//$('#menu-toggle-control-container').css('display', 'none');
		$('#menu-toggle-control-container').css('width', 0);
	    } else {

		//$('#menu-toggle-control-container').css('display', 'block');
		$('#menu-toggle-control-container').css('width', null);
	    }
	}

	$(window).resize(function() {

		smartPhoneHandler($);
	    });

	smartPhoneHandler($);
	
	/**
	 * Popover widgets
	 *
	 */
	$('#search-modal-help').popover();
	$('#share-modal-help').popover();
	$('#auth-modal-help').popover();
	$('#search-facets').popover({
		
		html: true,
		    placement: 'bottom',
		    content: function() {

		    $('#search-facets-content').show();
		    return $('#search-facets-content').html();
		}
	    });

	/**
	 * Affixed navbar
	 *
	 */
	if($('.navbar-inner').length > 0) {

	    $('.navbar-inner').affix({
		
		    offset: {
		    
			top: $('.navbar-inner').offset().top,
			    }
		});
	}

	/**
	 * Work-arounds handling feature requests for the responsive navbar
	 *
	 */

	if($( window ).width() <= 1156 ) {

	    if( $( window ).width() > 1024) {

		//$('.menu-toggle-container').css('height', 0);
	    } else {

		//$('.menu-toggle-container').height('height', '54px');
	    }
	} else {

	    //$('.menu-toggle-container').height('height', '54px');
	}

	/**
	 * Carousel implementation
	 *
	 */

	$('#carousel-featured-collection .carousel-inner .item').click(function(e) {

		$('#carousel-featured-collection').carousel('pause');
	    });
	$('#carousel-featured-collection').carousel('cycle');

    };

    // Ensure that the execution of all bootstrap functionality lies within a modular, Drupal-compliant context
    Drupal.behaviors.bootstrapDssLdr = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssLdr');
	}
    };

    /**
     * Work-around
     * @todo Investigate why this became necessary on 01/17/14
     *
     */
    $(window).load(function() {

	    Drupal.theme('bootstrapDssLdr');
	});

})(jQuery, Drupal);
