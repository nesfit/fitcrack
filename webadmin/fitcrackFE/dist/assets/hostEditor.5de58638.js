<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/hostEditor.5de58638.js
import{h as r}from"./hostSelector.1b477e48.js";import{n}from"./index.d3f441ac.js";var i=function(){var t=this,e=t.$createElement,s=t._self._c||e;return s("v-dialog",{attrs:{"max-width":"1200",value:t.value},on:{input:function(o){return t.$emit("input")}}},[s("v-card",[s("v-card-title",[s("span",[t._v("Host Mapping")]),s("v-spacer"),s("v-btn",{attrs:{icon:""},on:{click:function(o){return t.$emit("input")}}},[s("v-icon",[t._v("mdi-close")])],1)],1),s("v-card-text",{staticClass:"scroller"},[s("host-selector",{attrs:{"select-all":"","auto-refresh":""},model:{value:t.hosts,callback:function(o){t.hosts=o},expression:"hosts"}})],1),s("v-card-actions",{staticClass:"pa-6"},[s("v-btn",{attrs:{text:"",to:"/hosts"}},[s("v-icon",{attrs:{left:""}},[t._v(" mdi-desktop-classic ")]),t._v(" Host List ")],1),s("v-spacer"),s("b",{staticClass:"mr-4"},[t._v(t._s(t.hosts.length)+" selected")]),s("v-btn",{attrs:{id:"host-mapper-assign",loading:t.saving,color:"success"},on:{click:t.save}},[s("v-icon",{attrs:{left:""}},[t._v(" mdi-check ")]),t._v(" Assign ")],1)],1)],1)],1)},c=[];const l={components:{hostSelector:r},props:{value:Boolean,jobIds:{type:Array,required:!0}},data(){return{hosts:[],saving:!1}},methods:{async save(){this.saving=!0;try{await this.axios.post(this.$serverAddr+"/job/host",{job_ids:this.jobIds,newHost_ids:this.hosts.map(t=>t.id)}),this.$emit("input"),this.$emit("reload")}catch(t){console.error(t)}finally{this.saving=!1}}}},a={};var v=n(l,i,c,!1,_,"169b4914",null,null);function _(t){for(let e in a)this[e]=a[e]}var u=function(){return v.exports}();export{u as h};
========
import{h as r}from"./hostSelector.f160872a.js";import{n}from"./index.c11e1020.js";var i=function(){var t=this,e=t.$createElement,s=t._self._c||e;return s("v-dialog",{attrs:{"max-width":"1200",value:t.value},on:{input:function(o){return t.$emit("input")}}},[s("v-card",[s("v-card-title",[s("span",[t._v("Host Mapping")]),s("v-spacer"),s("v-btn",{attrs:{icon:""},on:{click:function(o){return t.$emit("input")}}},[s("v-icon",[t._v("mdi-close")])],1)],1),s("v-card-text",{staticClass:"scroller"},[s("host-selector",{attrs:{"select-all":"","auto-refresh":""},model:{value:t.hosts,callback:function(o){t.hosts=o},expression:"hosts"}})],1),s("v-card-actions",{staticClass:"pa-6"},[s("v-btn",{attrs:{text:"",to:"/hosts"}},[s("v-icon",{attrs:{left:""}},[t._v(" mdi-desktop-classic ")]),t._v(" Host List ")],1),s("v-spacer"),s("b",{staticClass:"mr-4"},[t._v(t._s(t.hosts.length)+" selected")]),s("v-btn",{attrs:{id:"host-mapper-assign",loading:t.saving,color:"success"},on:{click:t.save}},[s("v-icon",{attrs:{left:""}},[t._v(" mdi-check ")]),t._v(" Assign ")],1)],1)],1)],1)},c=[];const l={components:{hostSelector:r},props:{value:Boolean,jobIds:{type:Array,required:!0}},data(){return{hosts:[],saving:!1}},methods:{async save(){this.saving=!0;try{await this.axios.post(this.$serverAddr+"/job/host",{job_ids:this.jobIds,newHost_ids:this.hosts.map(t=>t.id)}),this.$emit("input"),this.$emit("reload")}catch(t){console.error(t)}finally{this.saving=!1}}}},a={};var v=n(l,i,c,!1,_,"169b4914",null,null);function _(t){for(let e in a)this[e]=a[e]}var u=function(){return v.exports}();export{u as h};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/hostEditor.87575981.js
