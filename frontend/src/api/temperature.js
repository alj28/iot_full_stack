
import axios from 'axios'
import {
    URL_BACKEND
} from './urls'


export function get_latest_temperature(device) {
    const URL = URL_BACKEND + 'temperature-log/' + device + '/';
    return axios.get(URL)
        .then((response) => {
            return response.data[response.data.length - 1].temperature;
        })
        .catch((error) => {
            throw error;
        });
}