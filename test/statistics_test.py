import unittest
import pedsp.statistics as statistics
import numpy as np
import scipy.stats
import scipy.stats.mstats
from utiltity import generate_inputs


class TestStatisticsMethods(unittest.TestCase):

    __number_inputs = 10
    __maximum_size = 1 << 14
    __minimum_size = 1 << 6

    # TODO: test entropy
    # TODO: test median
    # TODO: implement this list https://www.programcreek.com/python/example/66766/scipy.stats.kurtosis

    def test_harmonic_mean(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            data = data + 0.1
            generated = statistics.harmonic_mean(data)
            reference = scipy.stats.mstats.hmean(data)
            self.assertAlmostEqual(generated, reference)

    def test_generalized_mean(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            data = data + 0.1
            generated = statistics.geometric_mean(data)
            reference = scipy.stats.mstats.gmean(data)
            self.assertAlmostEqual(generated, reference)

    def test_mean(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            generated = statistics.mean(data)
            reference = np.mean(data)
            self.assertAlmostEqual(generated, reference.item())

    def test_variance(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            generated = statistics.variance(data)
            reference = np.var(data)
            self.assertAlmostEqual(generated, reference.item())

    def test_std(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            generated = statistics.standard_deviation(data)
            reference = np.std(data)
            self.assertAlmostEqual(generated, reference.item())

    def test_skewness(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            generated = statistics.skewness(data)
            reference = scipy.stats.mstats.skew(data)
            self.assertAlmostEqual(generated, reference.item())

    def test_kurtosis(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            generated = statistics.kurtosis(data)
            reference = scipy.stats.mstats.kurtosis(data, fisher=False)
            self.assertAlmostEqual(generated, reference.item())

    def test_moment(self):
        for data in generate_inputs(self.__number_inputs, self.__minimum_size, self.__maximum_size):
            for i in range(2, 8):
                generated = statistics.moment(data, i)
                reference = scipy.stats.mstats.moment(data, i)
                self.assertAlmostEqual(generated, reference)

