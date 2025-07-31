<template>
  <v-autocomplete
    v-bind="$attrs"
    @input="$emit('input', $event)"
    :value="value"
    :autofocus="autofocus"
    clearable
    :items="hashTypes"
    item-text="name"
    return-object
    hide-details
    no-data-text="No matching hash type"
  >
    <template #item="{ item }">
      <v-list-item-content>
        <v-list-item-title><b>{{ item.code }}</b> - {{ item.name }}</v-list-item-title>
      </v-list-item-content>
    </template>
  </v-autocomplete>
</template>

<script>
  export default {
    name: 'HashTypeAutocomplete',
    props: {
      autofocus: {
        type: Boolean,
        default: false
      },
      value: {
        type: Object,
        default: () => {}
      }
    },
    data () {
      return {
        hashTypes: []
      }
    },
    mounted () {
      this.loadHashTypes()
    },
    methods: {
      loadHashTypes () {
        this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => {
          this.hashTypes = response.data.hashtypes
        })
      },
    }
  }
</script>