(function($) {
  Drupal.behaviors.content_dashboard = {
    attach: function(context, settings) {
      $("form#content-dashboard-new-content select#edit-content-type-select").change(function(e) {
        window.location.pathname = 'admin/content/content-dashboard/new/' + $("option:selected", this).val();
      });
      $("form#content-dashboard-updated-content select#edit-content-type-select").change(function(e) {
        window.location.pathname = 'admin/content/content-dashboard/sync/' + $("option:selected", this).val();
      });
      if ($('table#content-dashboard-new-table tr:last').index() + 1 > 1) {
        $('#content-dashboard-new-table').before('<input type="search" autosave="content-dashboard-new-table" results=5 class="light-table-filter" data-table="tablesorter" placeholder="Search within table" style="border:1px solid black"size=60>');
        $("#content-dashboard-new-table").tablesorter({
          sortList: [
            [4, 1],
            [1, 0]
          ],
          widgets: ['zebra']
        });
      }
      if ($('table#content-dashboard-sync-table tr:last').index() + 1 > 1) {
        $('#content-dashboard-sync-table').before('<input type="search" autosave="content-dashboard-sync-table" results=5 class="light-table-filter" data-table="tablesorter" placeholder="Search within table" style="border:1px solid black" size=60>');
        $("#content-dashboard-sync-table").tablesorter({
          sortList: [
            [5, 1],
            [1, 0]
          ],
          widgets: ['zebra']
        });
      }
      if ($('table#content-dashboard-taxonomy-sync-table tr:last').index() + 1 > 1) {
        $('#content-dashboard-taxonomy-sync-table').before('<input type="search" autosave="content-dashboard-taxonomy-sync-table" results=5 class="light-table-filter" data-table="tablesorter" placeholder="Search within table" style="border:1px solid black" size=60>');
        $("#content-dashboard-taxonomy-sync-table").tablesorter({
          sortList: [
            [4, 1],
            [1, 0]
          ],
          widgets: ['zebra']
        });
      }
    }
  };
})(jQuery);