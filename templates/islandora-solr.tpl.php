<?php
/**
 * @file
 * Islandora solr search primary results template file.
 *
 * Variables available:
 * - $results: Primary profile results array
 *
 * @see template_preprocess_islandora_solr()
 */
?>

<div class="islandora islandora-solr-search-results">
<?php if($display == 'list'): ?>

    <?php if (empty($results)): ?>

       <div class="no-results"><?php print t('Sorry, but your search returned no results.'); ?></div>
    <?php else: ?>

    <ol class="islandora-solr-search-result-list">
    <?php
      $row_result = 0;
      foreach($results as $key => $result):
    ?>

        <!-- Search result -->

	<!-- <li class="islandora-solr-search-result-item"> -->
	<li class="islandora-solr-search-result clear-block <?php print $row_result % 2 == 0 ? 'odd' : 'even'; ?>">
        <!-- <div class="islandora-solr-search-result clear-block <?php print $row_result % 2 == 0 ? 'odd' : 'even'; ?>"> -->
          <!-- Thumbnail -->
          <dl class="solr-thumb">
          <dt>
            <?php
              $image = '<img src="' . url($result['thumbnail_url'], array('query' => $result['thumbnail_url_params'])) . '" />';
              // Construct options array for l() function call.  Only include
              // what is needed.  Can accept standard url parameters and a
              // single anchor tag (fragment) at the end.
              $options = array(
                'html' => TRUE,
              );
              if (isset($result['object_url_params'])):
                $options['query'] = $result['object_url_params'];
              endif;
              if (isset($result['object_url_fragment'])):
                $options['fragment'] = $result['object_url_fragment'];
              endif;
              // Construct the thumbnail link.
              print l($image, $result['object_url'], $options);
            ?>
          </dt>
          <dd></dd>
        </dl>
        <!-- Metadata -->
        <dl class="solr-fields islandora-inline-metadata">
          <?php
            $row_field = 0;
            $max_rows = count($results[$row_result]) - 1;
            foreach($result['solr_doc'] as $key => $value): ?>
            <dt class="solr-label
              <?php
                print $value['class'];
                print $row_field == 0 ? ' first' : '';
                print $row_field == $max_rows ? ' last' : '';
              ?>">
              <?php print $value['label']; ?>
            </dt>
            <?php
              if ($key == 'PID'):
                // Construct options array for l() function call.  Only include
                // what is needed.  Can accept standard url parameters and a
                // single anchor tag (fragment) at the end.
                $options = array(
                  'html' => TRUE,
                );
                if (isset($result['object_url_params'])):
                  $options['query'] = $result['object_url_params'];
                endif;
                if (isset($result['object_url_fragment'])):
                  $options['fragment'] = $result['object_url_fragment'];
                endif;
                // Construct the PID link.
                $value['value'] = l($value['value'], $result['object_url'], $options);
              endif;
            ?>
            <dd class="solr-value <?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?><?php print $row_field == $max_rows ? ' last' : ''; ?>">

		<!-- @griffinj For formatting date time values -->
		<?php print preg_match('/(?:Lower|Upper)/', $value['label']) ? date('F, Y', strtotime($value['value'])) : $value['value'] ?>
            </dd>
            <?php $row_field++; ?>
          <?php endforeach; ?>
        </dl>
      <!-- </div> --><!-- /.islandora-solr-search-result -->
    </li><!-- /.islandora-solr-search-result-item -->

    <?php endforeach; ?>
    </ol><!-- /.islandora-solr-search-result-list -->

  <?php endif; ?>
  <?php else: ?>

    <?php if (empty($results)): ?>

       <div class="no-results"><?php print t('Sorry, but your search returned no results.'); ?></div>
    <?php else: ?>

    <div class="islandora islandora-basic-collection">
    <div class="islandora-basic-collection-grid clearfix">
      <?php
        $row_result = 0;
        foreach($results as $key => $result):
      ?>

      <dl class="islandora-basic-collection-object">
          <dt class="islandora-basic-collection-thumb">
              <?php
                $image = '<img src="' . url($result['thumbnail_url'], array('query' => $result['thumbnail_url_params'])) . '" />';
                // Construct options array for l() function call.  Only include
                // what is needed.  Can accept standard url parameters and a
                // single anchor tag (fragment) at the end.
                $options = array('html' => TRUE,);
              if (isset($result['object_url_params'])):
                $options['query'] = $result['object_url_params'];
              endif;
              if (isset($result['object_url_fragment'])):
                $options['fragment'] = $result['object_url_fragment'];
              endif;
              // Construct the thumbnail link.
              print l($image, $result['object_url'], $options);
            ?>
        </dt>
        <dd class="islandora-basic-collection-caption">

	      <?php

		print $result['solr_doc']['dc.title']['value']; ?>
	</dd>
    </dl><!--/.islandora-basic-collection-object -->
  
  <?php

    $row_result++;
    endforeach;
  ?>
  </div><!-- /.islandora-basic-collection-grid -->
</div><!-- /.islandora-basic-collection-grid -->

<?php endif; ?><!-- Results -->
<?php endif; ?><!-- List/Grid view -->
</div><!-- /.islandora islandora-solr-search-results -->
