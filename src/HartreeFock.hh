#ifndef HartreeFock_h
#define HartreeFock_h

#include "ModelSpace.hh"
#include "Operator.hh"
#include "IMSRGProfiler.hh"
#include <armadillo>
#include <map>
#include <deque>

class HartreeFock
{
 public:
   Operator& Hbare;         ///< Input bare Hamiltonian
   ModelSpace * modelspace; ///< Model Space of the Hamiltonian
   arma::mat C;             ///< transformation coefficients, 1st index is ho basis, 2nd = HF basis
   arma::mat rho;           ///< density matrix rho_ij
   arma::mat KE;            ///< kinetic energy
   arma::mat Vij;           ///< 1 body piece of 2 body potential
   arma::mat V3ij;          ///< 1 body piece of 3 body potential
   arma::mat F;             ///< Fock matrix
   array< array< arma::mat,2>,3> Vmon;          ///< Monopole 2-body interaction
   array< array< arma::mat,2>,3> Vmon_exch;          ///< Monopole 2-body interaction
   arma::uvec holeorbs;     ///< list of hole orbits for generating density matrix
   arma::rowvec hole_occ; /// occupations of hole orbits
   arma::vec energies;      ///< vector of single particle energies
   arma::vec prev_energies; ///< SPE's from last iteration
   double tolerance;        ///< tolerance for convergence
   double EHF;              ///< Hartree-Fock energy (Normal-ordered 0-body term)
   double e1hf;             ///< One-body contribution to EHF
   double e2hf;             ///< Two-body contribution to EHF
   double e3hf;             ///< Three-body contribution to EHF
   int iterations;          ///< iterations used in Solve()
   vector<uint64_t> Vmon3_keys;
   vector< double> Vmon3;
   IMSRGProfiler profiler;  ///< Profiler for timing, etc.
   deque<double> convergence_ediff; ///< Save last few convergence checks for diagnostics
   deque<double> convergence_EHF; ///< Save last few convergence checks for diagnostics
   bool freeze_occupations;

// Methods
   HartreeFock(Operator&  hbare); ///< Constructor
   void BuildMonopoleV();         ///< Only the monopole part of V is needed, so construct it.
   void BuildMonopoleV3();        ///< Only the monopole part of V3 is needed.
   void Diagonalize();            ///< Diagonalize the Fock matrix
   void UpdateF();                ///< Update the Fock matrix with the new transformation coefficients C
   void UpdateDensityMatrix();    ///< Update the density matrix with the new coefficients C
   void FillLowestOrbits();       ///< Get new occupations based on the current single-particle energies
   bool CheckConvergence();       ///< Compare the current energies with those from the previous iteration
   void Solve();                  ///< Diagonalize and UpdateF until convergence
   void CalcEHF();                ///< Evaluate the Hartree Fock energy
   void PrintEHF();               ///< Print out the Hartree Fock energy
   void ReorderCoefficients();    ///< Reorder the coefficients in C to eliminate phases etc.
   Operator TransformToHFBasis( Operator& OpIn); ///< Transform an operator from oscillator basis to HF basis
   Operator GetNormalOrderedH();  ///< Return the Hamiltonian in the HF basis at the normal-ordered 2body level.
   Operator GetNormalOrderedH(arma::mat& Cin);  ///< Return the Hamiltonian in the HF basis at the normal-ordered 2body level.
   Operator GetOmega();           ///< Return a generator of the Hartree Fock transformation
   Operator GetHbare(){return Hbare;}; ///< Getter function for Hbare
   void PrintSPE(); ///< Print out the single-particle energies
   void FreeVmon();               ///< Free up the memory used to store Vmon3.
   void GetRadialWF(index_t index, vector<double>& R, vector<double>& PSI); ///< Return the radial wave function of an orbit in the HF basis
   double GetRadialWF_r(index_t index, double R); ///< Return the radial wave function of an orbit in the HF basis
   void FreezeOccupations(){freeze_occupations = true;};
   void UnFreezeOccupations(){freeze_occupations = false;};
   uint64_t Vmon3Hash(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f);
   void Vmon3UnHash(uint64_t key, int& a, int& b, int& c, int& d, int& e, int& f);

};



#endif

