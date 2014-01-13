<?php

/**
 * @file
 * islandora-basic-collection-wrapper.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
?>

<div class="islandora-basic-collection-wrapper">
  <div class="islandora-basic-collection clearfix">
    <span class="islandora-basic-collection-display-switch">

    <!-- Refactor -->
    <!-- <?php print theme('links', array('links' => $view_links, 'attributes' => array('class' => array('links', 'inline'))));?> -->
    <?php print l('Grid view', url("islandora/object/{$islandora_object->id}/pages", array('absolute' => TRUE)),
					  array('attributes' => array('class' => "islandora-view-grid active",))) ;?>
    <?php print l('List view', url("islandora/object/{$islandora_object->id}/pages", array('absolute' => TRUE)),
					  array('attributes' => array('class' => "islandora-view-grid active",))) ;?>
    <?php print l('List view', '<front>') ;?>

    </span>
    <?php print $collection_pager; ?>
    <?php print $collection_content; ?>
    <?php print $collection_pager; ?>
  </div>
</div>
