#ifndef KALMAN_HPP
#define KALMAN_HPP

template <typename T>
class Kalman
{
    T _q;	// bruit du processus
    T _r;	// bruit du capteur
    T _k;	// gain de Kalman
    T _val;	// la valeur
    T _p;	// estimation de la covariance de l'erreur

public:
    /** @brief Constructeur */
    Kalman(T q, T r, T initVal);

    /** @brief Calcul d'une valeur filtrée */
    T get(T val);
};

/**
 * @param q			bruit du processus
 * @param r			bruit du capteur
 * @param initVal	valeur initiale
 */
template <typename T>
Kalman<T>::Kalman(T q, T r, T initVal)
    : _q(q), _r(r), _val(initVal), _p(0)
{
}

/**
 * Cette méthode va effectuer le calcul de l'algorithme de filtrage de Kalman.
 * C'est une version simplifiée du filtre s'il n'y a qu'une seule valeur
 * @param val
 * @return
 */
template <typename T>
T Kalman<T>::get(T val)
{
    // maj de la prédiction
    _p += _q;

    // maj de la mesure
    _k    = _p / (_p + _r);
    _val += _k * (val - _val);
    _p   *= (1 - _k);

    return _val;
}

#endif // KALMAN_HPP







