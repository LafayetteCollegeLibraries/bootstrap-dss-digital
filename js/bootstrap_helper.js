/**
 * jQuery functionality for the theme
 * @author griffinj@lafayette.edu
 *
 */

(function($, Drupal) {

    // Following the Drupal.theme implementation
    // Please see https://drupal.org/node/304258
    Drupal.theme.prototype.bootstrapDssLdr = function() {



	$('.nav-collapse').on('show.bs.collapse', function() {

		$('.nav-collapse .nav').show();

		//$('.navbar-inner-container').addClass('opened').insertAfter($('.navbar-collapse-toggle'));
		$('.navbar-inner-container').addClass('opened');
		$('.navbar-inner-container').insertAfter($('.navbar-collapse-toggle'));


	    }).on('hide.bs.collapse', function() {

		    // Refactor, terrible hack
		    if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion) ) {

			$('.nav-collapse .nav').hide();
		    }

		    //$('.navbar-inner-container').removeClass('opened').insertBefore($('.auth-share-simple-search-container'));
		    $('.navbar-inner-container').removeClass('opened');
		    $('.navbar-inner-container').insertBefore($('.auth-share-simple-search-container'));

		});

	// Trigger the collapse for ...
	
	// Refactor, terrible hack
	if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion) ) {

	    //$('.navbar-inner-container').insertAfter($('.navbar-collapse-toggle'));
	    //$('.navbar-inner-container').insertAfter($('.navbar-inner'));

	    //$('div.navbar-collapse-toggle').css('float', 'right');
	    //$('div.nav-collapse nav ul.menu').css('width', '100%');

	    $('div.navbar-collapse-toggle div .btn-navbar').show();
	    $('.nav-collapse').collapse('toggle');

	    $('.nav-collapse .nav').hide();
	    $('.nav-collapse .nav li').addClass('collapsed');
	}

	// Work-around: The collapse widget appears to be broken
	$('.navbar-toggle').click(function() {

		// For navbar toggle
		$('.in').on('show.bs.collapse', function() {

			//$('.navbar-collapse-toggle').appendTo($('.navbar-inner-container'));
			$('.navbar-inner-container').appendTo($('.navbar-collapse-toggle'));
		    }).on('hide.bs.collapse', function() {

			    //$('.navbar-collapse-toggle').prependTo($('.navbar-inner-container'));
			    $('.navbar-inner-container').prependTo($('.navbar-collapse-toggle'));
			});

		$('.collapse').collapse('toggle');

	    });

	/**
	 * Popover widgets
	 *
	 */
	$('#share-modal-help').popover();
	$('#auth-modal-help').popover();

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

	// Refactor
	/* var maxWidth = 1322; */
	var maxWidth = 1340;

	/* var minWidth = 692; */
	var minWidth = 726;

	var responsiveWidth = 964;
	/* var responsiveWidth = 1340; */
	
	if($( window ).width() <= maxWidth ) {

	    //$('header#navbar').addClass('navbar-static-width');
	}

	$(window).resize(function() {

		console.log( $( window ).width());

		// Refactor, terrible hack
		if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion) ) {

		} else {

		    if($( window ).width() <= maxWidth ) {

			// Atrocious; refactor
			if( $( window ).width() > responsiveWidth ) {

			    $('header#navbar').addClass('navbar-static-width');
			    $('.auth-share-simple-search-container').removeClass('collapsed');
			    $('.nav-collapse .nav li').removeClass('collapsed');
		  
			} else {
			    
			    $('header#navbar').removeClass('navbar-static-width');
			    $('.auth-share-simple-search-container').addClass('collapsed');
			    $('.nav-collapse .nav li').addClass('collapsed');
			}
		    } else {
			
			$('header#navbar').removeClass('navbar-static-width');
		    }

		    if( $( window ).width() < minWidth ) {

			$('header#navbar').addClass('navbar-static-width-min');
		    } else {
			
			$('header#navbar').removeClass('navbar-static-width-min');
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
		}
	    });

	/**
	 * Handling for the simple search widget for smartphone browsing
	 *
	 */
	//if( /Android.*(?:Mobile)|webOS|iPhone|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion)) {
	if( /Android.*(?:Mobile)|webOS|iPhone|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.appVersion) || $(window).width() <= 320 ) {

	    // Avoid race conditions
	    // Decouple into a widget
	    $(document).data('bootstrapDssDigital.mobileSearch', false);

	    $('#simple-search-control-container a').click(function(e) {

		    if( $('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav ul.menu #islandora-solr-simple-search-form').length ) {
			//if( $(document).data('bootstrapDssDigital.mobileSearch') ) {

			$('#simple-search-item').remove();
			$('#navbar .navbar-inner .navbar-collapse-toggle div').click();
			$('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav .menu li').show();

			$(document).data('bootstrapDssDigital.mobileSearch', false);
		    } else {

			$simpleSearchMenuItem = $('<li id="simple-search-item" class="first leaf active btn"></li>').append($('#islandora-solr-simple-search-form').clone());

			$('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav ul.menu').children('li').hide();
			$('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav ul.menu').prepend($simpleSearchMenuItem.show());

			$('#navbar .navbar-inner .navbar-collapse-toggle .btn-navbar').click();

			$(document).data('bootstrapDssDigital.mobileSearch', true);
		    }
		});

	    $('.navbar-collapse-toggle .btn-navbar').click(function(e) {

		    //if( $('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav ul.menu #islandora-solr-simple-search-form').length ) {

		    // html.js body.html header#navbar.navbar div.navbar-inner div.navbar-inner-container div.nav-collapse
		    if( $('#navbar .navbar-inner .navbar-inner-container .nav-collapse.in').length ) {
		    //if( $(document).data('bootstrapDssDigital.mobileSearch')  ) {

			$(document).data('bootstrapDssDigital.mobileSearch', false);

			$('#simple-search-control-container a:visible').click();

			//$('#simple-search-item').remove();
			//$('#navbar .navbar-inner .navbar-collapse-toggle div').click();
			//$('#navbar .navbar-inner .navbar-inner-container .nav-collapse nav .menu li').show();
		    }
		});
	}
	
    }

    // Ensure that the execution of all bootstrap functionality lies within a modular, Drupal-compliant context
    Drupal.behaviors.bootstrapDssLdr = {

	attach: function(context, settings) {

	    Drupal.theme('bootstrapDssLdr');
	}
    }
})(jQuery, Drupal);
