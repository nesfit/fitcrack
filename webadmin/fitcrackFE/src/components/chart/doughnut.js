import { Doughnut } from 'vue-chartjs'
import deepMerge from 'deepmergefn'

export default {
  extends: Doughnut,
  props: {
    chartdata: {
      type: Object,
      default: null
    },
    options: {
      type: Object,
      default: null
    }
  },
  mounted () {
    this.renderChart(this.chartdata, deepMerge({
      maintainAspectRatio: false,
      tooltips: {
        mode: "index",
        intersect: true
      }
    }, this.options))
  }
}