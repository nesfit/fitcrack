<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="info != null"
      :items="
        [
          { text: 'Hashlists', to: { name: 'hashlists' }, exact: true },
          { text: info.name }
        ]"
      divider="/"
    />

    <v-container v-if="info != null">
      <v-card flat class="mb-4">
        <v-card-title>
          {{ info.name }}
        </v-card-title>
        <v-card-text>
          Hash type: <strong>{{ info.hash_type_name }}</strong>
        </v-card-text>
        <v-card-actions>
          <v-btn
            :to="{name: 'hashlistExtend', params: {id: info.id}}"
            color="primary"
            :disabled="info.is_locked"
          >
            <v-icon left>mdi-plus</v-icon>
            Add hashes
          </v-btn>
          <v-spacer></v-spacer>
          <v-btn
            @click="deleteHashlist"
            color="error"
            text
          >
            <v-icon left>mdi-delete</v-icon>
            Discard or restore
          </v-btn>
        </v-card-actions>
      </v-card>
      <v-card flat class="mb-4 transparent">
        <v-card-title>Jobs</v-card-title>
        <v-card-text v-if="info.jobs.length > 0" class="d-flex overflow-x-auto">
          <miniJob
            v-for="job in info.jobs"
            :key="job.id"
            :data="job"
          >
          </miniJob>
        </v-card-text>
        <v-card-actions>
          <v-btn
            @click="attachToJob"
            color="primary"
          >
            <v-icon left>mdi-briefcase-plus</v-icon>
            Add to a new job
          </v-btn>
        </v-card-actions>
      </v-card>
      <HashTable :id="$route.params.id"></HashTable>
    </v-container>
  </div>
</template>

<script>
  import fmt from '@/assets/scripts/numberFormat'
  import miniJob from '@/components/dashboard/miniJob.vue'
  import {mapMutations} from "vuex"
import HashTable from './hashTable.vue'

  export default {
    name: "HashlistDetail",
    components: {
    miniJob,
    HashTable
},
    data: function () {
      return {
        info: null
      }
    },
    mounted: function () {
      this.loadHashlistData()
    },
    methods: {
      ...mapMutations('jobForm', ['hashListIdMut', 'stepMut']),
      fmt,
      loadHashlistData() {
        this.axios.get(this.$serverAddr + '/hashlist/' + this.$route.params.id).then((response) => {
            this.info = response.data
            this.loading = false
        });
      },
      attachToJob() {
        this.hashListIdMut(this.$route.params.id)
        this.stepMut(2)
        this.$router.push({name: 'addJob'})
      },
      deleteHashlist() {
        this.axios.delete(this.$serverAddr + '/hashlist/' + this.$route.params.id).then((response) => {
          this.$router.push({name: 'hashlists'})
        })
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 800px;
  }
  .dictContent {
    max-height: 600px;
  }
  .cursor-pointer {
    cursor: pointer
  }

</style>
