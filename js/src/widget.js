/**
 * @file Drupal widgets
 * A Drupal widget
 *
 */

/**
 * Drupal behaviors for Modules...
 * Please note that these behaviors are integrated with jQuery, and are called bound to DOM events by the loading of the DOM
 *
 */
Drupal.behaviors.myModuleBehavior = {

    attach: function (context, settings) {

	$('input.myCustomBehavior', context).once('myCustomBehavior', function () {

		// Apply the myCustomBehaviour effect to the elements only once.
	    });
    }
};

/**
 * Drupal behaviors for Themes...
 * Please note that these methods must be invoked by using client-side script
 * @usage Drupal.theme('myThemeFunction', arg1, arg2);
 *
 */
Drupal.theme.prototype.myThemeFunction = function (arg1, arg2) {

    return null;
};
