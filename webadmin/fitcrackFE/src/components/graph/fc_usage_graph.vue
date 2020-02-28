<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="graphContainer">
    <div
      v-if="data === null"
      class="loading"
    >
      <v-progress-circular
        indeterminate
        color="primary"
      />
    </div>
    <div
      v-else-if="data.length === 0"
      class="loading"
    >
      <h1>No data</h1>
    </div>
    <div v-else>
      <line-chart
        :id="id"
        class="maxh200"
        style="height: 200px"
        :data="data"
        :labels="labelsText"
        grid="true"
        xkey="time"
        :ykeys="labels"
        hide-hover="false"
        resize="true"
        :smooth="true"
        :line-colors="['#1a50a3', '#00b752', '#dc3043', '#eecd3b', '#768ea2', '#b300ec', '#a9d2f9', '#ffd8b1', '#008080', '#d2f53c']"
      />
    </div>
  </div>
</template>

<script>
    import Raphael from 'raphael/raphael'
    import {LineChart} from 'vue-morris'

    global.Raphael = Raphael;


    export default {
        name: "FcUsageGraph",
        components: {
            LineChart,
        },
        props: ['data', 'id', 'type', 'labels', 'labelsText']
    }
</script>

<style scoped>
    .maxh200{
        max-height: 200px;
    }

    .graphContainer {
        width: 100%;
    }

    .loading {
        text-align: center;
        padding: 20px;
        vertical-align: middle;
        font-weight: 200;
    }
</style>