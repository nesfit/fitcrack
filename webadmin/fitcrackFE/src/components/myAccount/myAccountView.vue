<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-container fluid>
      <h2> My account </h2>
      <div class="table">
      <v-card class="width150">
          <v-card-title>
            <h3> My info </h3>
          </v-card-title>
          <v-card-text>

            <v-form ref="form" lazy-validation>
              <v-text-field
                label="username"
                :value = "this.username"
              ></v-text-field>
              <v-text-field
                label="email"
                :value = "this.email"
              ></v-text-field>
            </v-form>
          </v-card-text>


      </v-card>

      </div>

      <div class="table">
      <v-card class="width150">
        <v-card-title>
          <h3> Change password </h3>
        </v-card-title>
        <v-card-text>

      <v-form ref="form" lazy-validation>
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
          </v-form>
        </v-card-text>
          <v-card-actions>
            <v-spacer></v-spacer>
            <v-btn color="primary" flat @click.stop="editPassword" :disabled="this.newPassword0 != this.newPassword1 || this.oldPassword == null || this.newPassword0 == null">Update</v-btn>
          </v-card-actions>

    </v-card>
  </div>
  <!--    <h3> Message: {{ user }} </h3> -->
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
            console.log(response.data);
            console.log('user password changed');
            this.oldPassword = '';
            this.newPassword0 = '';
            this.newPassword1 = ''
          })
        },
        loadUsername() {
          this.axios.get(this.$serverAddr + '/user/isLoggedIn').then((response) => {
            this.user = response.data.items;
            this.username=response.data.user.username;
            this.email=response.data.user.mail
          })


        }
      },
      data: function() {
       return {
          username: "",
          email: "",
          newPassword0: null,
          newPassword1: null,
          user: []
        }
      }
  }
</script>



<style scoped>
h2 {
  padding-bottom: 5px;
  text-align: center;
}
.table {
  max-width: 500px;
  margin: auto;
}
.width150 {
  min-width: 100px;
}

</style>
