import { Doughnut, mixins } from 'vue-chartjs'
import deepMerge from 'deepmergefn'

export default {
  extends: Doughnut,
  mixins: [mixins.reactiveProp],
  props: {
    options: {
      type: Object,
      default: null
    }
  },
  mounted () {
    this.renderChart(this.chartData, deepMerge({
      maintainAspectRatio: false,
      tooltips: {
        mode: "index",
        intersect: true
      }
    }, this.options))
  }
}