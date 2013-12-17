/**
 * jQuery functionality for the theme
 * @author griffinj@lafayette.edu
 *
 */

(function($, Drupal) {

    // Following the Drupal.theme implementation
    // Please see https://drupal.org/node/304258
    Drupal.theme.prototype.bootstrapDssLdr = function() {

	// Work-around: The collapse widget appears to be broken
	$('.navbar-toggle').click(function() {

		$('.collapse').collapse('toggle');
	    });

	// For the popovers
	$('#share-modal-help').popover();
	$('#auth-modal-help').popover();

	// For the header
	//$('#navbar').affix({
	/*
	$('.navbar-header').affix({

		offset: {

		    top: 30,
			bottom: 5

		}
	    });
	*/

	/*
	$('#navbar-header').on('activate.bs.scrollspy', function () {

		console.log('trace');
	    });
	*/

	/*
	$('.navbar-inner').affix({
		
		offset: {

		    top: $('.navbar-inner').offset().top,
		    
			/*
		    bottom: function() {

			return $('.navbar-inner').offset().top;
		    }
			* /
		}
	    });
	*/

	if($('.navbar-inner').length > 0) {

	$('.navbar-inner').affix({
		
		offset: {
		    
		    top: $('.navbar-inner').offset().top,
		}
	    });
	}

	//$('.dropdown-submenu').dropdown('toggle').click(function(e) {
	/*
	$('.dropdown-menu li .dropdown-menu').dropdown('toggle').click(function(e) {

		e.preventDefault();
		$(this).dropdown();
	    });
	*/

	/*
	$('.dropdown-menu li .dropdown-menu').each(function(i, e) {

		console.log( $(e));
		$(e).addClass('dropdown-submenu');

		$(e).dropdown('toggle').parent().click(function(e) {

			e.preventDefault();
			$(this).dropdown();
		    });
	    });
	*/

	//$('.dropdown-submenu').dropdown('toggle');
	/*
	$('.dropdown-submenu').on('show.bs.dropdown', function(e) {

		console.log('trace');
	    });
	*/

	// Refactor
	var maxWidth = 1322;
	var responsiveWidth = 932;

	if($( window ).width() <= maxWidth ) {

	    $('header#navbar').addClass('navbar-static-width');
	}

	$(window).resize(function() {

		if($( window ).width() <= maxWidth ) {

		    // Atrocious; refactor
		    if( $( window ).width() > responsiveWidth ) {

			$('header#navbar').addClass('navbar-static-width');
		    } else {

			$('header#navbar').removeClass('navbar-static-width');
		    }
		} else {
		    
		    $('header#navbar').removeClass('navbar-static-width');
		}
		
		if($( window ).width() <= 754 ) {

		    // Refactor
		    if($('#navbar .navbar-header h1 a').text() != 'DSS') {

			$(document).data('Drupal.theme.bootstrap.dss', $('#navbar .navbar-header h1 a').text());
			$('#navbar .navbar-header h1 a').text('DSS');
		    }
		    
		    //$('header#navbar').addClass('navbar-static-width');
		} else {

		    //$(document).data('Drupal.theme.bootstrap.dss', $('#navbar .navbar-header h1 a').text());
		    if($('#navbar .navbar-header h1 a').text() == 'DSS') {

			$('#navbar .navbar-header h1 a').text( $(document).data('Drupal.theme.bootstrap.dss'));
		    }
		    
		    //$('header#navbar').removeClass('navbar-static-width');
		}
	    });
	
    }

    // Ensure that the execution of all bootstrap functionality lies within a modular, Drupal-compliant context
    Drupal.behaviors.bootstrapDssLdr = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssLdr');
	}
    }
})(jQuery, Drupal);
