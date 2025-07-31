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
          <div>Created at: <strong>{{ new Date(info.added).toLocaleString() }}</strong> UTC</div>
          <div>Hash type: <strong>{{ info.hash_type_name }}</strong></div>
          <div class="d-flex align-center">
            <span class="mr-2">Progress: <strong>{{ info.cracked_hash_count }} / {{ info.hash_count }} ({{ Math.round(percentProgress) }} %)</strong></span>
            <v-progress-circular :value="percentProgress" :max="100" color="success"
              size="18" rotate="270" />
          </div>
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
            color=""
            text
          >
            <v-icon left>{{ info.deleted ? 'mdi-delete-restore' : 'mdi-delete' }}</v-icon>
            {{ info.deleted ? 'Restore' : 'Discard' }}
          </v-btn>
          <v-btn
            @click="purgeHashlistConfirm"
            color="error"
            text
          >
            <v-icon left>mdi-undo</v-icon>
            Purge
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
      <HashTable :id="parseInt($route.params.id)"></HashTable>
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
    computed: {
      percentProgress() {
        return this.info.cracked_hash_count / this.info.hash_count * 100
      }
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
      },
      purgeHashlistConfirm() {
        this.$root.$confirm('Purge Hashlist', `This will undo all cracking progress for ${this.info.name}. Any attached and running job will be killed with it! Are you sure?`)
          .then((confirm) => {
            this.purgeHashlist()
          })
      },
      purgeHashlist() {
        this.axios.post(this.$serverAddr + '/hashlist/' + this.$route.params.id + '/purge').then((response) => {
          this.loadHashlistData()
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
