<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-container fluid>
      <h2> My account </h2>
      <h3> Change password </h3>

      <v-form ref="form" lazy-validation>
  <!--      <v-text-field
          label="username"
          :placeholder= "username"
        ></v-text-field>
        <v-text-field
            label="email"
            :placeholder= "email"
          ></v-text-field> -->
          <v-text-field
            type="password"
            label="Old password"
            v-model="oldPassword"
            required
          ></v-text-field>
          <v-text-field
            type="password"
            label="New password"
            v-model="newPassword0"
            required
          ></v-text-field>
          <v-text-field
            type="password"
            label="New password"
            v-model="newPassword1"
            required
          ></v-text-field>
          <v-card-actions>
            <v-spacer></v-spacer>
            <v-btn color="primary" flat @click.stop="editPassword" :disabled="this.newPassword0 != this.newPassword1">Edit</v-btn>
          </v-card-actions>
      </v-form>
      <h3> Message: {{ username }} </h3>
    </v-container>
</template>

<script>
    export default {
      name: "myAccount",
      computed: {
        gradient: function () {
          return window.theme.gradient
        }
      },

      methods: {
    /*    loadUsername() {
          this.axios.get(this.$serverAddr + '/user/').then((response) => {
            this.username = response.data.items
          })
        },*/
        editPassword() {
          this.axios.post(this.$serverAddr + '/user/password/change_my_password', {
            old_password: this.oldPassword,
            new_password: this.newPassword1
          }).then((response) => {
            this.oldPassword = '';
            this.newPassword0 = '';
            this.newPassword1 = ''
          })
        }

      },
      data: function() {
       return {
          username: this.$username,
          email: "mail",
          newPassword0: null,
          newPassword1: null
        }
      }
  }
</script>



<style scoped>
h2 {
  padding-bottom: 5px;
  text-align: center;
}
</style>
