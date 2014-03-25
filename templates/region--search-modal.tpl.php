<?php

  /**
   * @file
   * The template for the search_modal region
   *
   */
?>

<?php if ($content): ?>

  <div class="lafayette-dss-modal" id="advanced-search-modal" tabindex="-1" role="dialog" aria-labelledby="Advanced Search Modal" aria-hidden="true" data-backdrop="static">
    <div class="modal-dialog">
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close advanced-search-modal-close" data-dismiss="modal" data-width="560" aria-hidden="true">Close</button>
	  <h4 class="modal-title">Advanced Search</h4>
        </div>
        <div class="modal-body">

          <?php print $content; ?>
          <button id="search-modal-help" class="btn btn-default" data-content="Search help message." data-placement="bottom" data-toggle="popover" data-container="body" type="button"><i class="icon-large" title="Click for more information"></i></button>
        </div>
<<<<<<< HEAD
          <button id="search-modal-help" class="btn btn-default" data-content="To add multiple search terms to your query, please use the \"+\" button above.  Each one you add can be set to a different field using the dropdown to the left of each, and you can also choose whether or not each field will be joined by an \"and,\" \"or,\" or \"not\" operator." data-placement="bottom" data-toggle="popover" data-container="body" type="button"><i class="icon-large" title="Click for more information"></i></button>
=======
>>>>>>> parent of 5ee006e... DSSSM-598 #time 17m added text to both
      </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
  </div><!-- /.modal -->

<?php endif; ?>
