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

<div class="islandora-solr-content content">

    <div class="islandora-discovery-controls">

      <div class="islandora-discovery-inner-container">

      <div class="islandora-page-controls">
        <form id="islandora-discovery-form" action="/" >
      </div><!--/.islandora-page-controls -->

    <div class="islandora-discovery-control page-number-control">

<span>Show:</span>
<select>
<option>25</option>
</select>
    </div><!-- /.islandora-discovery-control -->

    <div class="islandora-discovery-control title-sort-control">

<span>Sort by:</span>
<select>
<option>Title</option>
</select>
    </div><!-- /.islandora-discovery-control -->

    </form>
    <?php print $collection_pager; ?>
  </div><!-- /.islandora-discovery-control -->

    <span class="islandora-basic-collection-display-switch">

      <ul class="links inline">
    <?php foreach ($view_links as $label => $link): ?>
    <li>
    <span id="view-<?php print $label ?>-icon" ></span>
    <?php print $link; ?>
  </li>
  <?php endforeach ?>
  </ul>
  </span>

  </div><!-- /.islandora-discovery-controls -->


    <?php print $collection_content; ?>
    <?php print $collection_pager; ?>
  </div>
</div>
