<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <transition name="fade-transition">
    <v-jumbotron
      :gradient="gradient"
      class="jumbo"
      height="100%"
    >
      <v-container fluid fill-height>
        <v-layout flex align-center justify-center>
          <v-card raised class="jumboCard px-4">
            <img :src="require(`@/assets/${$vuetify.theme.project}.png`)" class="mx-auto pt-3 pb-2 d-block logo" alt="logo"/>
            <div class="content">
              <v-progress-circular
                size="50"
                :width="3"
                class="progress trans"
                indeterminate
                color="primary"
                v-bind:class="{hide: !loading}"
              ></v-progress-circular>
              <v-form ref="form" lazy-validation v-bind:class="{hide: loading}" class="trans">
                <v-text-field
                  label="Username"
                  v-model="username"
                  required
                ></v-text-field>
                <v-text-field
                  label="Password"
                  v-model="password"
                  required
                  type="password"
                ></v-text-field>
                <v-btn
                  class="d-block mx-auto mb-4"
                  flat
                  outline
                  type="submit"
                  color="primary"
                  v-on:click.prevent="submit"
                >
                  submit
                </v-btn>
              </v-form>
            </div>
          </v-card>
        </v-layout>
      </v-container>
    </v-jumbotron>
  </transition>
</template>

<script>
  export default {
    name: 'loginView',
    data: function () {
      return {
        username: null,
        password: null,
        loading: true
      }
    },
    created: function () {
      this.isUserLoggedIn()
    },
    computed: {
      gradient: function () {
        return window.theme.gradient
      }
    },
    methods: {
      submit() {
        this.loading = true;
        // Native form submission is not yet supported
        this.axios.post(this.$serverAddr + '/user/login', {
          username: this.username,
          password: this.password
        }).then((response) => {
          this.loading = false;
          console.log(response.data)
          this.$logInUser(response.data)

        })
      },
      isUserLoggedIn: function () {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/user/isLoggedIn').then((response) => {
          this.loading = false;
          if (response.data.loggedIn) {
            this.$logInUser(response.data.user)
          }
        })
      }
    }
  }
</script>
<style scoped>
  .hide {
    opacity: 0;
    visibility: hidden;
  }

  .trans {
    -webkit-transition: opacity .3s ease-in-out;
    -moz-transition: opacity .3s ease-in-out;
    -ms-transition: opacity .3s ease-in-out;
    -o-transition: opacity .3s ease-in-out;
    transition: opacity .3s ease-in-out;
  }

  .content {
    position: relative;
  }

  .progress {
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: auto;
    position: absolute;
  }

  .jumbo {
    position: fixed;
    z-index: 5;
  }

  .logo {
    width: 100%;
    max-width: 200px;
  }
</style>

