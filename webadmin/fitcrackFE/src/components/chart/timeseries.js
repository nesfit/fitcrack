import { Line } from 'vue-chartjs'
import deepMerge from 'deepmergefn'
import { gridColor } from './helpers'

export default {
  extends: Line,
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
      elements: {
        line: {
          tension: 0,
          fill: false
        }
      },
      scales: {
        x: {
          type: "time",
          gridLines: {
            color: gridColor
          }
        },
        y: {
          gridLines: {
            color: gridColor
          }
        }
      },
      tooltips: {
        mode: "nearest",
        intersect: false
      }
    }, this.options))
  }
}