var c=Object.defineProperty;var n=Object.getOwnPropertySymbols;var u=Object.prototype.hasOwnProperty,m=Object.prototype.propertyIsEnumerable;var a=(t,i,e)=>i in t?c(t,i,{enumerable:!0,configurable:!0,writable:!0,value:e}):t[i]=e,o=(t,i)=>{for(var e in i||(i={}))u.call(i,e)&&a(t,e,i[e]);if(n)for(var e of n(i))m.call(i,e)&&a(t,e,i[e]);return t};import{n as d}from"./index.c11e1020.js";import"./vendor.6aa46313.js";var h=function(){var t=this,i=t.$createElement,e=t._self._c||i;return e("v-container",{staticClass:"rmw mt-4"},[e("v-row",[e("v-col",[e("v-card",{attrs:{flat:""}},[e("v-card-title",[e("v-icon",{attrs:{left:""}},[t._v(" mdi-palette ")]),e("span",[t._v("Appearance")])],1),e("v-card-text",[e("v-btn-toggle",{attrs:{mandatory:"",color:"primary"},model:{value:t.appearance,callback:function(s){t.appearance=s},expression:"appearance"}},[e("v-btn",{attrs:{value:"auto"}},[e("v-icon",{staticClass:"neutral ml-0",attrs:{left:"",small:""}},[t._v(" mdi-creation ")]),e("span",[t._v("Automatic")])],1),e("v-btn",{attrs:{value:"light"}},[e("v-icon",{staticClass:"neutral ml-0",attrs:{left:"",small:""}},[t._v(" mdi-brightness-5 ")]),e("span",[t._v("Light")])],1),e("v-btn",{attrs:{value:"dark"}},[e("v-icon",{staticClass:"neutral ml-0",attrs:{left:"",small:""}},[t._v(" mdi-brightness-2 ")]),e("span",[t._v("Dark")])],1)],1),e("div",{staticClass:"mt-2 ml-1"},[t._v(" "+t._s(t.appearanceHint)+"."),e("span",{directives:[{name:"show",rawName:"v-show",value:t.testmode,expression:"testmode"}]},[t._v(" Temporarily toggle with CTRL+SHIFT+L anywhere.")])])],1),e("v-card-title",[e("v-icon",{attrs:{left:""}},[t._v(" mdi-cursor-pointer ")]),e("span",[t._v("Behavior")])],1),e("v-card-text",[e("v-row",[e("v-col",{staticClass:"mw"},[e("v-switch",{staticClass:"mb-4 mt-0",attrs:{label:"Job kill confirmation",hint:"Job kill button will ask for confirmation to prevent accients.","persistent-hint":""},model:{value:t.confirmpurge,callback:function(s){t.confirmpurge=s},expression:"confirmpurge"}}),e("v-switch",{staticClass:"mb-4 mt-0",attrs:{loading:t.loading,outlined:"",label:"Verify hash format",hint:"Check if the format of every user-entered hash is valid.","persistent-hint":""},model:{value:t.settings.verify_hash_format,callback:function(s){t.$set(t.settings,"verify_hash_format",s)},expression:"settings.verify_hash_format"}})],1),e("v-col",{staticClass:"mw"},[e("v-switch",{staticClass:"mb-4 mt-0",attrs:{loading:t.loading,outlined:"",label:"Add new hosts to running jobs",hint:"Automatically assign newly connected hosts to currently running jobs.","persistent-hint":""},model:{value:t.settings.auto_add_hosts_to_running_jobs,callback:function(s){t.$set(t.settings,"auto_add_hosts_to_running_jobs",s)},expression:"settings.auto_add_hosts_to_running_jobs"}}),e("v-switch",{staticClass:"mb-4 mt-0",attrs:{loading:t.loading,outlined:"",label:"Fully benchmark new hosts",hint:"Run a complete first-time benchmark on new hosts connected to the system.","persistent-hint":""},model:{value:t.settings.bench_all,callback:function(s){t.$set(t.settings,"bench_all",s)},expression:"settings.bench_all"}})],1)],1)],1)],1),e("v-expansion-panels",{staticClass:"mt-6",attrs:{flat:""}},[e("v-expansion-panel",[e("v-expansion-panel-header",{staticClass:"px-4",scopedSlots:t._u([{key:"default",fn:function(s){var l=s.open;return[e("span",{staticClass:"d-flex align-center"},[e("v-icon",{attrs:{left:""}},[t._v(" mdi-puzzle ")]),e("span",{staticClass:"text-h6"},[t._v(t._s(l?"":"Show ")+"Advanced Settings")])],1)]}}])}),e("v-expansion-panel-content",[e("v-card-text",[e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",label:"Default time per workunit",min:10,hint:t.wuTimeHint,color:t.settings.default_seconds_per_workunit<t.wutthresh?"warning":"","persistent-hint":"",suffix:"seconds"},model:{value:t.settings.default_seconds_per_workunit,callback:function(s){t.$set(t.settings,"default_seconds_per_workunit",s)},expression:"settings.default_seconds_per_workunit"}}),e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",min:"5",label:"Workunit timeout factor",hint:"Multiplying factor for workunit timeout \u2014 the time after which a workunit is considered failed.","persistent-hint":""},model:{value:t.settings.workunit_timeout_factor,callback:function(s){t.$set(t.settings,"workunit_timeout_factor",s)},expression:"settings.workunit_timeout_factor"}}),e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",label:"Distribution coefficient \u03B1",hint:"Maximum percentage of the remaining keyspace that can be assigned with a single workunit unless it would be below the minimum.","persistent-hint":""},model:{value:t.settings.distribution_coefficient_alpha,callback:function(s){t.$set(t.settings,"distribution_coefficient_alpha",s)},expression:"settings.distribution_coefficient_alpha"}}),e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",label:"T_pmin",min:"10",max:"3600",hint:"Absolute minimum seconds per workunit (including the start of hashcat, etc.). Prevents creation of extremely small workunits.","persistent-hint":"",suffix:"seconds"},model:{value:t.settings.t_pmin,callback:function(s){t.$set(t.settings,"t_pmin",s)},expression:"settings.t_pmin"}}),e("v-switch",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",label:"Ramp-up workunits",hint:"If enabled, Fitcrack creates smaller workunits at start. The size increases until the solving time hits the \u201CTime per workunit\u201D value.","persistent-hint":""},model:{value:t.settings.ramp_up_workunits,callback:function(s){t.$set(t.settings,"ramp_up_workunits",s)},expression:"settings.ramp_up_workunits"}}),e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",min:"0.0",max:"1.0",label:"Ramp down coefficient",hint:"Minimum fraction of \u201CTime per workunit\u201D that can be created. Influences the size of workunits at the end of the job. The lower the value, the smaller the size workunits at the end. 1.0 means no ramp down, 0.0 ramp down is limited only by T_pmin.","persistent-hint":""},model:{value:t.settings.ramp_down_coefficient,callback:function(s){t.$set(t.settings,"ramp_down_coefficient",s)},expression:"settings.ramp_down_coefficient"}}),e("v-text-field",{staticClass:"mb-4",attrs:{loading:t.loading,outlined:"",type:"number",label:"Temperature threshold",hint:"Abort cracking if temperature of the client's PC reaches this threshold","persistent-hint":"",suffix:"\xB0C"},model:{value:t.settings.hwmon_temp_abort,callback:function(s){t.$set(t.settings,"hwmon_temp_abort",s)},expression:"settings.hwmon_temp_abort"}}),e("v-switch",{staticClass:"mb-4",attrs:{label:"Developer mode",hint:"Enables useful utilities throughout the app when developing Fitcrack.","persistent-hint":""},model:{value:t.testmode,callback:function(s){t.testmode=s},expression:"testmode"}})],1)],1)],1)],1)],1)],1)],1)},p=[];const f={name:"Settings",data:function(){return{appearance:localStorage.getItem("appearance")||"auto",testmode:localStorage.getItem("testmode")=="true"||!1,settings:{},loading:!0,saving:!1,wutthresh:180,confirmpurge:!localStorage.hasOwnProperty("confirmpurge")||localStorage.getItem("confirmpurge")=="true"}},computed:{appearanceHint(){switch(this.appearance){case"auto":return"Webadmin will change appearance dynamically based on your system theme";case"light":return"Webadmin will retain light appearance regardless of your system theme";case"dark":return"Webadmin will retain dark appearance regardless of your system theme";default:return}},wuTimeHint(){return this.settings.default_seconds_per_workunit&&this.settings.default_seconds_per_workunit<this.wutthresh?"Setting extremely low time per workunit leads to high overhead. Cracking may take much longer than estimated. Consider choosing a higher default value.":"The time per workunit preference used for new jobs. You can change it on a per-job basis in the additional settings step when creating a job or via the edit job dialog."}},watch:{appearance(t){localStorage.setItem("appearance",t),t=="dark"&&(this.$vuetify.theme.dark=!0),t=="light"&&(this.$vuetify.theme.dark=!1),t=="auto"&&(this.$vuetify.theme.dark=window.matchMedia("(prefers-color-scheme: dark)").matches)},testmode(t){localStorage.setItem("testmode",t)},confirmpurge(t){localStorage.setItem("confirmpurge",t)},settings:{deep:!0,immediate:!1,handler(t,i){!this.loading&&Object.keys(i).length>0&&this.saveSettings()}}},mounted(){this.loadSettings()},beforeRouteLeave(t,i,e){this.saving||e()},methods:{async loadSettings(){this.loading=!0,this.settings=await this.axios.get(this.$serverAddr+"/settings").then(t=>t.data),this.loading=!1},saveSettings(){if(this.settings.ramp_down_coefficient<0){this.$error("Ramp down coefficient must be higher or equal to 0.0.");return}if(this.settings.ramp_down_coefficient>1){this.$error("Ramp down coefficient must be smaller or equal to 1.0.");return}if(this.settings.t_pmin<10){this.$error("Absolute minimum seconds per workunit must be higher or equal to 10.");return}if(this.settings.t_pmin>3600){this.$error("Absolute minimum seconds per workunit must be smaller or equal to 3600.");return}if(this.settings.workunit_timeout_factor<5){this.$error("Workunit timeout factor cannot be smaller than 5.");return}this.saving=!0,this.axios.post(this.$serverAddr+"/settings",o({},this.settings)).then(()=>{this.saving=!1,this.$store.state.jobForm.timeForJob=this.settings.default_seconds_per_workunit})}}},r={};var g=d(f,h,p,!1,_,"0864e542",null,null);function _(t){for(let i in r)this[i]=r[i]}var k=function(){return g.exports}();export{k as default};
