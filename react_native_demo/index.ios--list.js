/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */
var MOCKED_MOVIES_DATA = [
  {title: 'Title', year: '2015', posters: {thumbnail: 'http://www.52ij.com/uploads/allimg/160317/1110104P8-4.jpg'}},
];

var REQUEST_URL = 'https://raw.githubusercontent.com/facebook/react-native/master/docs/MoviesExample.json';

import React, {
  AppRegistry,
  Component,
  NetInfo,
  Image,
  ListView,
  StyleSheet,
  Text,
  View
} from 'react-native';


class AwesomeProject extends Component {

  constructor(props) {
    super(props);
    this.state = {
      dataSource: new ListView.DataSource({
        rowHasChanged:(row1,row2) => row1 !== row2,
      }),
      
      loaded: false,
    };
  }

   handleFirstConnectivityChange(reach) {
      console.log('First change: ' + reach);

    }
    

  componentDidMount() {
    this.fetchData();


    NetInfo.addEventListener(
      'change',
      this.handleFirstConnectivityChange
    );
  }

  fetchData() {
    fetch(REQUEST_URL)
    .then((response) => response.json())
    .then((responseData) => {
      this.setState({
        dataSource: this.state.dataSource.cloneWithRows(responseData.movies),
        loaded: true,
      });
    }).done();
  }

  render() {

    if (!this.state.loaded) {
      return this.renderLoadingView(); 
    };

    return (
      <ListView
        dataSource = {this.state.dataSource}
        renderRow = {this.renderMovie}
       style={styles.listView}>

       </ListView>

    );
  }

  renderLoadingView() {
    return (
      <View style={styles.container}>
        <Text>
        Loading movies...
        </Text>
      </View>
      );
  }

  renderMovie(movie) {

    return (
        <View style={styles.container}>
          <Image source={{uri: movie.posters.thumbnail}} style={styles.thumbnail} />

          <View style={styles.rightContainer}>
            <Text style={styles.title} > {movie.title} </Text>
            <Text style={styles.year} > {movie.year} </Text>
          </View>

        </View>
      );
  }

}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },

  listView: {
    paddingTop: 20,
    backgroundColor: '#F5FCFF'
  },

  thumbnail: {
    width: 53,
    height: 81,
  },
  rightContainer: {
    flex: 1,
    // flexDirection: 'row',
    // justifyContent: 'flex-start',
    // alignItems: 'center',
  },
  title: {
    fontSize: 20,
    marginBottom: 8,
    textAlign: 'center'
  },
  year: {
    textAlign: 'center'
  },
});

AppRegistry.registerComponent('AwesomeProject', () => AwesomeProject);
