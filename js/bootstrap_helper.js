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

	$('.navbar-inner').affix({
		
		offset: {
		    
		    top: $('.navbar-inner').offset().top,
		}
	    });


	$('.dropdown-submenu').on('show.bs.dropdown', function(e) {
		
		e.preventDefault();
	    });
    }

    // Ensure that the execution of all bootstrap functionality lies within a modular, Drupal-compliant context
    Drupal.behaviors.bootstrapDssLdr = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssLdr');
	}
    }
})(jQuery, Drupal);