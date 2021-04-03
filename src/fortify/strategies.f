        module m_strategy_pattern
        use iso_c_binding
        implicit none

        type :: Strategy
            character(len=20) :: label
            procedure(generic_function), pointer, nopass :: on_submit
        contains
            procedure :: init
        end type Strategy

        abstract interface
            subroutine generic_function(numbers)
                integer, dimension(:), intent(in) :: numbers
            end subroutine
        end interface

        contains

            subroutine init(self, func, label)
                class(Strategy), intent(inout) :: self
                procedure(generic_function) :: func
                character(len=*) :: label

                self%on_submit => func      !! Procedure pointer
                self%label = label

            end subroutine init

            subroutine summation(array)
                integer, dimension(:), intent(in) :: array
                integer :: total
                total = sum(array)
                write(*,*) total
            end subroutine summation

            subroutine join(array)
                integer, dimension(:), intent(in) :: array
                write(*,*) array        !! Just write out the whole array
            end subroutine join

        end module m_strategy_pattern

        program test_strategy
        use m_strategy_pattern
        implicit none

            type(Strategy) :: summation_strat, join_strat
            integer :: i

            call summation_strat%init(summation, "Add them")
            call join_strat%init(join, "Join them")

            call summation_strat%on_submit([(i, i=1,10)])   !! Array constructor syntax
            call join_strat%on_submit([(i, i=1,10)])

        end program test_strategy