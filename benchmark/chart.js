/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

(function() {
  'use strict';

  var Hana = {};
  Hana.initChart = function(div, options) {
    if (options.xAxis == undefined) {
      options.xAxis = {
        title: { text: "Number of elements" },
        minTickInterval: 1
      };
    }

    if (options.yAxis == undefined) {
      options.yAxis = {
        title: { text: "Time (s)" },
        floor: 0
      };
    }

    if (options.subtitle == undefined) {
      options.subtitle = { text: "(smaller is better)" };
    }

    if (options.chart == undefined) {
      options.chart = { zoomType: 'xy' };
    }

    if (options.title.x == undefined) {
      options.title.x = -20; // center
    }

    if (options.series.stickyTracking == undefined) {
      options.series.stickyTracking = false;
    }

    options.tooltip = options.tooltip || {};
    options.tooltip.valueSuffix = options.tooltip.valueSuffix || 's';

    if (options.legend == undefined) {
      options.legend = {
        layout: 'vertical',
        align: 'right',
        verticalAlign: 'middle',
        borderWidth: 0
      };
    }
    div.highcharts(options);
  };

  window.Hana = Hana;
})();
