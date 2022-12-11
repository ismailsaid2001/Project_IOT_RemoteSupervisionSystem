// Import the functions you need from the SDKs you need
import {
    initializeApp
} from "https://www.gstatic.com/firebasejs/9.14.0/firebase-app.js";
import {
    getAuth,
    createUserWithEmailAndPassword
} from 'https://www.gstatic.com/firebasejs/9.14.0/firebase-auth.js';
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
submitData.addEventListener('click', () => {
    var email = document.getElementById('email').value;
    var password = document.getElementById('psw').value;
    createUserWithEmailAndPassword(auth, email, password)
        .then((userCredential) => {
            // Signed in
            const user = userCredential.user;
            // ...
        })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;
            alert(errorMessage);
            // ..
        });

});