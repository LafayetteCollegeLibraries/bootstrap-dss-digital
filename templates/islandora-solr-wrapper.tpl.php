<?php

/**
 * @file islandora-solr-wrapper.tpl.php
 * Islandora solr search results wrapper template
 *
 * Variables available:
 * - $variables: all array elements of $variables can be used as a variable. e.g. $base_url equals $variables['base_url']
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $secondary_profiles: Rendered secondary profiles
 * - $results: Rendered search results (primary profile)
 * - $islandora_solr_result_count: Solr result count string
 * - $solrpager: The pager
 * - $solr_debug: debug info
 *
 * @see template_preprocess_islandora_solr_wrapper()
 */
?>

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
<option value="">Title</option>
<option value=".islandora-inline-metadata dd.solr-value.eastasia-coverage-location">Coverage.Location</option>
</select>
    </div><!-- /.islandora-discovery-control -->
    </form>

    <span class="islandora-basic-collection-display-switch">
      <ul class="links inline">
        <?php foreach ($view_links as $link): ?>
          <li>

            <a <?php print drupal_attributes($link['attributes']) ?>><?php print filter_xss($link['title']) ?></a>
            <img src="<?php print $view_icon_srcs[$link['title']]; ?>" alt="<?php print $view_icon_alts[$link['title']] ?>" id="<?php print $view_icon_ids[$link['title']] ?>" />
          </li>
        <?php endforeach ?>
      </ul>
    </span><!-- /.islandora-basic-collection-display-switch -->

    <div class="pagination-count">
    <div class="islandora-result-count">
      <?php print $islandora_solr_result_count; ?>
    </div><!-- /.islandora-result-count -->


    <?php print $solr_pager; ?>
    </div><!-- /.pagination-count -->

  </div><!-- /.islandora-discovery-inner-container -->
  </div><!-- /.islandora-discovery-controls -->
  <?php print $results; ?>
  <?php print $solr_debug; ?>
  <?php print $solr_pager; ?>
</div>