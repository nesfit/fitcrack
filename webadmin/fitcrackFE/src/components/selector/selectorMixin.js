export default {
  props: {
    selectAll: {
      type: Boolean,
      default: false
    },
    value: {
      type: Array,
      default: function () {
        return []
      }
    }
  },
  data() {
    return {
      items: [],
      loading: false,
      search: '',
      selected: []
    }
  },
  mounted() {
    this.getData()
  },
  methods: {
    getData() {
      console.error('Selector component missing getData method')
    },
    updateSelected() {
      this.$emit('input', this.selected)
    }
  }
}