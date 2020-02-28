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
    if (this.value) this.selected = this.value
  },
  methods: {
    getData() {
      console.error('Selector component missing getData method')
    },
    updateSelected() {
      this.$emit('input', this.selected)
    }
  },
  watch: {
    value (val) {
      this.selected = val
    }
  }
}