import { Doughnut, mixins } from 'vue-chartjs'
import deepMerge from 'deepmerge'

export default {
  extends: Doughnut,
  mixins: [mixins.reactiveProp],
  props: {
    overrides: {
      type: Object,
      default: null
    }
  },
  computed: {
    options () {
      return deepMerge({
        maintainAspectRatio: false,
        tooltips: {
          mode: "index",
          intersect: true
        }
      }, this.overrides)
    }
  },
  mounted () {
    this.renderChart(this.chartData, this.options)
  }
}