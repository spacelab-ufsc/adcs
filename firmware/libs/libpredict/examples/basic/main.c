#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <predict/predict.h>

predict_orbital_elements_t e;
predict_observer_t o;
struct predict_sgp4 g;
struct predict_sdp4 d;

int main( int argc, char ** argv )
{
    const char * tle_line_1 = "1 25544U 98067A   24223.83784911  .00020194  "
                              "00000+0  36238-3 0  9994";
    const char * tle_line_2 = "2 25544  51.6408  44.5872 0005770 185.1957 "
                              "306.5656 15.49872002467029";

    // Create orbit object
    predict_orbital_elements_t * iss = predict_parse_tle( &e,
                                                          &g,
                                                          &d,
                                                          tle_line_1,
                                                          tle_line_2 );
    if( !iss )
    {
        fprintf( stderr, "Failed to initialize orbit from tle!" );
        exit( 1 );
    }

    // Create observer object
    predict_observer_t * obs = predict_create_observer( &o,
                                                        "Me",
                                                        63.9 * M_PI / 180.0,
                                                        10.9 * M_PI / 180.0,
                                                        0 );
    if( !obs )
    {
        fprintf( stderr, "Failed to initialize observer!" );
        exit( 1 );
    }

    printf( "\e[1;1H\e[2J" ); // clear screen

    while( true )
    {
        printf( "\033[0;0H" ); // print from start of the terminal

        predict_julian_date_t curr_time = julian_from_timestamp( time( NULL ) );

        // Predict ISS
        struct predict_position iss_orbit;
        predict_orbit( iss, &iss_orbit, curr_time );
        printf( "ISS: lat=%f, lon=%f, alt=%f\n",
                iss_orbit.latitude * 180.0 / M_PI,
                iss_orbit.longitude * 180.0 / M_PI,
                iss_orbit.altitude );

        // Observe ISS
        struct predict_observation iss_obs;
        predict_observe_orbit( obs, &iss_orbit, &iss_obs );
        printf( "ISS: azi=%f (rate: %f), ele=%f (rate: %f)\n",
                iss_obs.azimuth * 180.0 / M_PI,
                iss_obs.azimuth_rate * 180.0 / M_PI,
                iss_obs.elevation * 180.0 / M_PI,
                iss_obs.elevation_rate * 180.0 / M_PI );

        // Apparent elevation
        double apparent_elevation = predict_apparent_elevation(
            iss_obs.elevation );
        printf( "Apparent ISS elevation: %.2f\n\n",
                apparent_elevation * 180.0 / M_PI );

        // Predict and observe MOON
        struct predict_observation moon_obs;
        predict_observe_moon( obs, curr_time, &moon_obs );
        printf( "MOON: %f, %f\n",
                moon_obs.azimuth * 180.0 / M_PI,
                moon_obs.elevation * 180.0 / M_PI );

        // Predict and observe SUN
        struct predict_observation sun_obs;
        predict_observe_sun( obs, curr_time, &sun_obs );
        printf( "SUN: %f, %f\n",
                sun_obs.azimuth * 180.0 / M_PI,
                sun_obs.elevation * 180.0 / M_PI );

        // Sleep
        fflush( stdout );
        usleep( 1000000 );
    }

    // Free memory
    predict_destroy_orbital_elements( iss );
    predict_destroy_observer( obs );

    return 0;
}
