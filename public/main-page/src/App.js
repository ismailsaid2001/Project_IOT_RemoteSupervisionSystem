import './App.css';
import pic from './tmp.png'
import gaz from './gaz.png'
import pres from './pres.png'
import hum from './hum.png'
import React from 'react';
import {Carousel} from '3d-react-carousal';
import {
  initializeApp
  } from "https://www.gstatic.com/firebasejs/9.14.0/firebase-app.js";
  import { getAuth, createUserWithEmailAndPassword,signInWithEmailAndPassword } from
  'https://www.gstatic.com/firebasejs/9.14.0/firebase-auth.js';
  import { getDatabase,child,ref,get } from "https://www.gstatic.com/firebasejs/9.14.0/firebase-database.js";
  // TODO: Add SDKs for Firebase products that you want to use
  // https://firebase.google.com/docs/web/setup#available-libraries

  // Your web app's Firebase configuration
  const firebaseConfig = {
  apiKey: "AIzaSyCpQTkmgendQZFmmCbNHXxrquSg1cgIjwU",
  authDomain: "project1-f66fe.firebaseapp.com",
  projectId: "project1-f66fe",
  storageBucket: "project1-f66fe.appspot.com",
  messagingSenderId: "890563503864",
  appId: "1:890563503864:web:fdc88df92979c42ca8bd0d"
  };

  // Initialize Firebase
  const app = initializeApp(firebaseConfig);
  const auth = getAuth(app);
  const db = getDatabase(app);
  const dbRef = ref(getDatabase());
  get(child(dbRef, 'Machine')).then((snapshot) => {
  if (snapshot.exists()) {
    const val=snapshot.val();
    console.log(val)
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});
function Number(props)
{
  return (
    <div className='value text-center d-flex flex-column align-items-center' >
    <p> {props.text}</p>
    </div>
  );
}
class Value extends React.Component{
    constructor(props)
    {
      super(props)
      
      
      this.state={temp : 22};

    }
    componentDidMount() {
      this.timerID = setInterval(
        () => this.update(),
        1000
      );
    }
    componentWillUnmount() {
      clearInterval(this.timerID);
    }
    update()
    {
      const app = initializeApp(firebaseConfig);
      const auth = getAuth(app);
      const db = getDatabase(app);
      const dbRef = ref(getDatabase());
      get(child(dbRef, 'Machine')).then((snapshot) => {
        var val=null
        if(this.props.value==="temp")
        var val=snapshot.val().temperature+"C°";
        else if(this.props.value==="gaz")
        var val=snapshot.val().gaz;
        else if(this.props.value==="pression")
        var val=snapshot.val().pression+"Mbr";
        else if(this.props.value==="hum")
        var val=snapshot.val().humidite+"%";
        console.log(val)
      this.setState({
        temp: val
      });
      console.log(this.state.temp)
    });
    }
      

    render()
    {
      return(<p>{this.state.temp}</p>)
    }
}

function App() {
  
  let slides = [ 
    <div className="py-5 bg-light text-success border border-success text-center d-flex flex-column align-items-center"><h3 className="mb-3 title">Temperature</h3>
    <Value value="temp"/>
    <img src={pic}></img>
    </div>,
    <div className="py-5 bg-light text-success border border-success text-center d-flex flex-column align-items-center"><h3 className="mb-3 title">Pression</h3>
    <Value value="pression"/>
    <img src={pres}></img>
    </div>,
    <div className="py-5 bg-light text-success border border-success text-center d-flex flex-column align-items-center"><h3 className="mb-3 title">Gaz</h3>
    <Value value="gaz"/>
    <img src={gaz}></img>
    </div>,
    <div className="py-5 bg-light text-success border border-success text-center d-flex flex-column align-items-center"><h3 className="mb-3 title">Humidite</h3>
    <Value value="hum"/>
    <img src={hum}></img>
    </div>,
];
  return (
    <div className="App">
      <div className='t'>
      <h1 className='welcome c'>Welcome Iheb!</h1>
      <a name="" id="" class="btn btn-outline-success c logout" href="C:\Users\si iheb\OneDrive\Desktop\TBARBISH\React\Tp_Smart_Devices\public\signin.html" role="button">LOG OUT</a>
      </div>
      <div className="Car">
      <Carousel slides={slides} autoplay={false} interval={1000} className="carousel"/>
      </div>
      
      </div>
    
  );
}
export default App;

