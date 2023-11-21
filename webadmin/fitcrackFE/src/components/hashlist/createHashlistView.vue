<template>
  <v-container>
    <v-row>
      <v-col>
        <v-alert
          v-show="$route.query.attach"
          icon="mdi-information"
          color="success"
        >
          Once created, this hashlist will be attached to your new job in the form.
        </v-alert>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <h2>Create a Hashlist</h2>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-text-field
          v-model="name"
          outlined
          :autofocus="name.length <= 0"
          required
          :rules="[value => !!value || 'Name is required']"
          label="Name"
          hint="Give this hashlist a descriptive name"
          persistent-hint
        />
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <HashlistCreator
          ref="creator"
          :hashlist-name="name"
          :autofocus="name.length > 0"
          @validation="val => validCreatorInput = val"
        />
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-btn
          large block
          @click="$router.go(-1)"
        >
          <v-icon left>
            mdi-arrow-u-left-top
          </v-icon>
          Cancel
        </v-btn>
      </v-col>
      <v-col>
        <v-btn 
          large block
          color="primary"
          :disabled="invalidInput"
          @click="create"
        >
          <v-icon left>
            {{ $route.query.attach ? 'mdi-briefcase-plus' : 'mdi-check' }}
          </v-icon>
          {{ $route.query.attach ? 'Create and attach to job' : 'Confirm' }}
        </v-btn>
      </v-col>
    </v-row>
    <!-- <v-row v-show="toDo.length > 0">
      <v-col>
        <h3>To-Do List</h3>
        <v-list>
          <v-list-item
            v-for="item in toDo"
            :key="item"
          >
            <v-icon
              left
              color="success"
            >
              mdi-check
            </v-icon>
            {{ item }}
          </v-list-item>
        </v-list>
      </v-col>
    </v-row> -->
  </v-container>
</template>

<script>
import HashlistCreator from './hashlistCreator.vue';
import { mapMutations } from 'vuex'

export default {
  name: "createHashlist",
  components: {
    HashlistCreator
  },
  data: function () {
    return {
      name: this.$route.query.attach || '',
      validCreatorInput: false
    }
  },
  computed: {
    invalidInput () {
      return this.name.length <= 0 || !this.validCreatorInput
    },
    toDo () {
      const todos = []
      if (this.name.length <= 0) todos.push("Name it.")
      return todos
    }
  },
  methods: {
    ...mapMutations('jobForm', ['hashListIdMut']),
    create () {
      const creator = this.$refs.creator
      creator.createHashlist()
        .then(result => {
          // maybe do something?
          //
          // if attaching, set the id in job form
          this.hashListIdMut(result.data.id)
          // return
          this.router.go(-1)
        })
        .catch(error => {
          console.error(error)
          this.$error(error)
        })
    }
  }
}
</script>