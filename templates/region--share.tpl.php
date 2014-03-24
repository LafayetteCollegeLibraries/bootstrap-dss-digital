<?php

  /**
   * @file
   * For theming the authentication region
   *
   */
?>

<?php if ($content): ?>

  <div class="lafayette-dss-modal" id="share-modal" tabindex="-1" role="dialog" aria-labelledby="Content Sharing Modal" aria-hidden="true" data-backdrop="static">
    <div class="modal-dialog">
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close share-modal-close" data-dismiss="modal" aria-hidden="true">Close</button>
	  <h4 class="modal-title">Share this Page</h4>
        </div>
        <div class="modal-body">

          <?php print render($content); ?>
          <button id="share-modal-help" class="btn btn-default" data-content="To post a link to the page of our site you are currently on to your Facebook or Twitter account, click either of these buttons to automatically do so." data-placement="bottom" data-toggle="popover" data-container="body" type="button"><i class="icon-large" title="Click for more information"></i></button>
        </div>


      </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
  </div><!-- /.modal -->
<?php endif; ?>
