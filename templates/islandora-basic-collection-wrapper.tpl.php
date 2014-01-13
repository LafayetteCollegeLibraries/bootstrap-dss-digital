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

    <?php $query_params = drupal_get_query_parameters($_GET); ?>
    <?php print l('Grid view', url("islandora/object/{$islandora_object->id}"),
		  array('attributes' => array('class' => "islandora-view-grid active"),
			'absolute' => TRUE,
			'query' => array_merge($query_params, array('display' => 'grid'))));?>
    <?php print l('List view', url("islandora/object/{$islandora_object->id}"),
		  array('attributes' => array('class' => "islandora-view-list"),
			'absolute' => TRUE,
			'query' => array_merge($query_params, array('display' => 'list'))));?>

    </span>
    <?php print $collection_pager; ?>
    <?php print $collection_content; ?>
    <?php print $collection_pager; ?>
  </div>
</div>
