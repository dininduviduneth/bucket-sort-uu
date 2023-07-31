# BUCKET SORT IMPLEMENTATION
This README section will explain how to run the code in different modes.

## 1. Array generation

The program to generate number arrays is in ```array_gen``` folder. The ```Makefile``` in it contains commands to create different types of arrays and save them in files. Comment off the unwanted ones and run ```make``` to automatically generate the files. Or simply run the ```./generate_array``` command in the following format to generate a custom array of your choice.

```
    ./generate_array N distribution_type relative_output_file_name.bin
```
```N``` could be any integer value. ```distribution_type``` should could be U for uniform distribution, N for normal distribution and E for exponential distribution.

## 2.Running the serial code

To run the serial bucket sorting program on a generated set of numbers,

* Navigate into ```serial``` folder and run ```make```.

* Run the ```./bucket``` program in the folliwing format.

    ```
    /bucket relative_input_file_path bucket_count
    ```
* If the code was run correctly, a file with the sorted set of numbers with the name ```results.bin``` will be created in the same directory.

* Add the desired number of buckets in the place of ```bucket_count```.

## 3. Running the parallel code

To run the parallel bucket sorting program on a generated set of numbers,

* Navigate into ```parallel``` folder and run ```make```.

* Run the ```./bucket``` program in the folliwing format.

    ```
    /bucket relative_input_file_path thread_count
    ```
* If the code was run correctly, a file with the sorted set of numbers with the name ```results.bin``` will be created in the same directory.

* Add the desired number of buckets in the place of ```thread_count```. In the parallel implementation, the ```thread_count``` will be taken as the number of buckets.

## 4. Validate the result

To validate if a generated file of numbers consists of accurately sorted numbers, navigate to ```check_accuracy``` folder and run ```make```.

* Run the ```./check_accuracy``` program in the folliwing format.

    ```
    /check_accuracy relative_path_of_result_file relative_path_of_original_file
    ```

The result will be displayed in the console.

## 5. View experiments

The experiments are saved in a python notebook named ```experiments.ipynb``` in the ```experiments``` directory. It also contains the ```csv``` files which contains the data gathered for experimentation.

## 6. Reset the directory

The directory can be reset (remove all generated files) by running ```make clean``` from the main directory path.

If required to clean files within the sub-directories, their versions of ```make clean``` can be used.